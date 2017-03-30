/* $Id: ncat_listen.c 13656 2009-06-10 16:11:07Z david $ */

#include "ncat.h"

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#else
#include <fcntl.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

static fd_set master, read_fds;
static fd_list_t fdlist;
static int listen_socket;
static int conn_count = 0;
/* Has stdin seen EOF? */
static int stdin_eof = 0;

static void handle_connection(void);
static int read_stdin(void);
static int read_socket(int recv_fd);

/* reap child processes */
static void sig_chld(int signo)
{
    while (Waitpid(-1, NULL, WNOHANG) > 0)
        conn_count--;
}

static int ncat_listen_tcp()
{
#ifdef HAVE_OPENSSL
    SSL_CTX *ctx;
#endif

    /* clear out structs */
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    zmem(&fdlist, sizeof(fdlist));

#ifndef WIN32
    /* Reap on SIGCHLD */
    Signal(SIGCHLD, sig_chld);
    /* Ignore the SIGPIPE that occurs when a client disconnects suddenly and we
       send data to it before noticing. */
    Signal(SIGPIPE, SIG_IGN);
#endif

#ifdef HAVE_OPENSSL
    if (o.ssl)
        ctx = setup_ssl_listen();
#endif

    /* setup the main listening socket */
    listen_socket = do_listen(SOCK_STREAM);

    /* Make our listening socket non-blocking because there are timing issues
     * which could cause us to block on accept() even though select() says it's
     * readable.  See UNPv1 2nd ed, p422 for more.
     */
    unblock_socket(listen_socket);

    /* setup select sets and max fd */
    FD_SET(listen_socket, &master);

    /* we need a list of fds to keep current fdmax and send data to clients */
    init_fdlist(&fdlist, sadd(o.conn_limit, 2));
    add_fd(&fdlist, listen_socket, NULL);
    add_fd(&fdlist, STDIN_FILENO, NULL);

    while (1) {
        int i, fds_ready;

        if(o.debug > 1)
            logdebug("selecting, fdmax %d\n", fdlist.fdmax);

        read_fds = master;
        fds_ready = fselect(fdlist.fdmax + 1, &read_fds, NULL, NULL, NULL);

        if(o.debug > 1)
            logdebug("select returned %d fds ready\n", fds_ready);

        /*
         * FIXME: optimize this loop to look only at the fds in the fd list,
         * doing it this way means that if you have one descriptor that is very
         * large, say 500, and none close to it, that you'll loop many times for
         * nothing.
         */
        for (i = 0; i <= fdlist.fdmax && fds_ready > 0; i++) {
            /* Loop through descriptors until there's something to read */
            if (!FD_ISSET(i, &read_fds))
                continue;

            if (o.debug > 1)
                logdebug("fd %d is ready\n", i);

            if (i == listen_socket) {
                /* we have a new connection request */
                handle_connection();
            } else if(i == STDIN_FILENO) {
                /* Read from stdin and write to all clients. */
                if (read_stdin() <= 0) {
                    /* There will be nothing more to send. If we're not
                       receiving anything, we can quit here. */
                    if (o.sendonly)
                        goto quit;
                }
            } else {
                /* Read from a client and write to stdout. */
                if (read_socket(i) <= 0) {
                    if (!o.keepopen)
                        goto quit;
                }
            }

            fds_ready--;
        }
    }

quit:
    return 0;
}

/* Accept a connection on a listening socket. Allow or deny the connection.
   Fork a command if o.cmdexec is set. Otherwise, add the new socket to the
   watch set. */
static void handle_connection(void)
{
    struct sockaddr_storage remoteaddr;
    socklen_t ss_len;
    int fd;
#ifdef HAVE_OPENSSL
    SSL *tmpssl = NULL;
#else
    void *tmpssl = NULL;
#endif

    ss_len = sizeof(remoteaddr);
    errno = 0;
    fd = accept(listen_socket, (struct sockaddr *) &remoteaddr, &ss_len);

    if (fd < 0) {
        if (o.debug)
            logdebug("Error in accept: %s\n", strerror(errno));

        close(fd);
        return;
    }

    if (o.verbose)
        loguser("Connection from %s.\n", inet_socktop(&remoteaddr));

    /* Check conditions that might cause us to deny the connection. */
    if (conn_count >= o.conn_limit) {
        if (o.verbose)
            loguser("New connection denied: connection limit reached (%d)\n", conn_count);
        Close(fd);
        return;
    }
    if (!allow_access(&remoteaddr)) {
        if (o.verbose)
            loguser("New connection denied: not allowed\n");
        Close(fd);
        return;
    }

    /* On Linux the new socket will be blocking, but on BSD it inherits the
       non-blocking status of the listening socket. The socket must be blocking
       for operations like SSL_accept to work in the way that we use them. */
    block_socket(fd);

#ifdef HAVE_OPENSSL
    if (o.ssl) {
        tmpssl = new_ssl(fd);
        if (SSL_accept(tmpssl) != 1) {
            if (o.verbose) {
                loguser("Failed SSL connection from %s: %s\n",
                        inet_socktop(&remoteaddr), ERR_error_string(ERR_get_error(), NULL));
            }
            SSL_free(tmpssl);
            Close(fd);
            return;
        }
    }
#endif

    conn_count++;

    /*
     * are we executing a command? if so then don't add this guy
     * to our descriptor list or set.
     */
    if (o.cmdexec) {
        netrun(fd, o.cmdexec);
    } else {
        /* add to our lists */
        FD_SET(fd, &master);
        /* Now that a client is connected, pay attention to stdin. */
        if (!stdin_eof)
            FD_SET(STDIN_FILENO, &master);
        /* add it to our list of fds for maintaining maxfd */
        if (add_fd(&fdlist, fd, tmpssl) < 0)
             bye("add_fd() failed.");
    }
}

/* Read from stdin and broadcast to all client sockets. Return the number of
   bytes read, or -1 on error. */
int read_stdin(void)
{
    int nbytes;
    char buf[DEFAULT_TCP_BUF_LEN];
    char* tempbuf = NULL;
    fd_set fds;

    nbytes = read(STDIN_FILENO, buf, sizeof(buf));
    if (nbytes <= 0) {
        if (nbytes < 0 && o.verbose)
            logdebug("Error reading from stdin: %s\n", strerror(errno));
        if (nbytes == 0 && o.debug)
            logdebug("EOF on stdin\n");

        /* Don't close the file because that allows a socket to be fd 0. */
        FD_CLR(STDIN_FILENO, &master);
        /* Buf mark that we've seen EOF so it doesn't get re-added to the
           select list. */
        stdin_eof = 1;

        return nbytes;
    }

    if (o.crlf)
        fix_line_endings((char *) buf, &nbytes, &tempbuf);

    if(o.linedelay)
        ncat_delay_timer(o.linedelay);

    /* Write to everything in the master set, except the listener, sender, and
       stdin. */
    fds = master;
    FD_CLR(STDIN_FILENO, &fds);
    FD_CLR(listen_socket, &fds);
    if (tempbuf != NULL) {
        broadcast(&fds, &fdlist, tempbuf, nbytes);
        free(tempbuf);
        tempbuf = NULL;
    } else
        broadcast(&fds, &fdlist, buf, nbytes);
    

    return nbytes;
}

/* Read from a client socket and write to stdout. Return the number of bytes
   read from the socket, or -1 on error. */
int read_socket(int recv_fd)
{
    char buf[DEFAULT_TCP_BUF_LEN];
    struct fdinfo *fdn = get_fdinfo(&fdlist, recv_fd);
    ssize_t nbytes;

    assert(fdn);

#ifdef HAVE_OPENSSL
readagain:
    if (o.ssl && fdn->ssl)
        nbytes = SSL_read(fdn->ssl, buf, sizeof(buf));
    else
#endif
        nbytes = recv(recv_fd, buf, sizeof(buf), 0);

    if (nbytes <= 0) {
        if (o.debug)
            logdebug("Closing connection.\n");

#ifdef HAVE_OPENSSL
        if (o.ssl && fdn->ssl) {
            if (nbytes == 0)
                SSL_shutdown(fdn->ssl);
            SSL_free(fdn->ssl);
        }
#endif

        close(recv_fd);
        FD_CLR(recv_fd, &master);
        rm_fd(&fdlist, recv_fd);

        conn_count--;
        if (conn_count == 0)
            FD_CLR(STDIN_FILENO, &master);

        return nbytes;
    }

    if(o.linedelay)
        ncat_delay_timer(o.linedelay);

    if (!o.sendonly) {
        if (o.telnet)
            dotelnet(recv_fd, (unsigned char *) buf, nbytes);

        Write(STDOUT_FILENO, buf, nbytes);
        ncat_log_recv(buf, nbytes);
    }

#ifdef HAVE_OPENSSL
    /* SSL can buffer our input, so doing another select()
     * won't necessarily work for us.  We jump back up to
     * read any more data we can grab now
     */
    if (o.ssl && fdn->ssl && SSL_pending(fdn->ssl))
        goto readagain;
#endif

    return nbytes;
}

/* This is sufficiently different from the TCP code (wrt SSL, etc) that it
 * resides in its own simpler function
 */
static int ncat_listen_udp()
{
    int sockfd, fdmax, nbytes, fds_ready;
    char buf[DEFAULT_UDP_BUF_LEN] = {0};
    char* tempbuf = NULL;
    fd_set master,  read_fds;
    struct sockaddr_storage remotess;
    socklen_t sslen = sizeof(remotess);

    FD_ZERO(&master);
    read_fds = master;

    /* Initialize remotess struct so recvfrom() doesn't hit the fan.. */
    zmem(&remotess, sizeof(remotess));
    remotess.ss_family = o.af;

#ifndef WIN32
    /* Reap on SIGCHLD */
    Signal(SIGCHLD, sig_chld);
#endif

    while (1) {
        /* create the UDP listen socket */
        sockfd = do_listen(SOCK_DGRAM);

        while (1) {
            /*
             * We just peek so we can get the client connection details without
             * removing anything from the queue. Sigh.
             */
            nbytes = Recvfrom(sockfd, buf, sizeof(buf), MSG_PEEK,
                                (struct sockaddr *) &remotess, &sslen);

            /* Check conditions that might cause us to deny the connection. */
            if (conn_count >= o.conn_limit) {
                if (o.verbose)
                    loguser("New connection denied: connection limit reached (%d)\n", conn_count);
            } else if (!allow_access(&remotess)) {
                if (o.verbose)
                    loguser("New connection denied: not allowed\n");
            } else {
                /* Good to go. */
                break;
            }

            /* Dump the current datagram */
            Recv(sockfd, buf, sizeof(buf), 0);
        }

        conn_count++;

        /*
         * We're using connected udp. This has the down side of only
         * being able to handle one udp client at a time
         */
        Connect(sockfd, (struct sockaddr *) &remotess, sslen);

        /* clean slate for buf */
        zmem(buf, sizeof(buf));

        /* are we executing a command? then do it */
        if (o.cmdexec) {
            netrun(sockfd, o.cmdexec);
            continue;
        }

        FD_SET(sockfd, &master);
        FD_SET(STDIN_FILENO, &master);
        fdmax = sockfd;

        /* stdin -> socket and socket -> stdout */
        while (1) {
            read_fds = master;

            if(o.debug > 1)
                logdebug("udp select'ing\n");

            fds_ready = fselect(fdmax + 1, &read_fds, NULL, NULL, NULL);

            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                nbytes = Read(STDIN_FILENO, buf, sizeof(buf));
                if(nbytes == 0)
                    return 0;
                if (o.crlf)
                    fix_line_endings((char *) buf, &nbytes, &tempbuf);
                if (!o.recvonly) {
                    if (tempbuf != NULL)
                        send(sockfd, tempbuf, nbytes, 0);
                    else
                        send(sockfd, buf, nbytes, 0);
                }
                if (tempbuf != NULL) {
                    free(tempbuf);
                    tempbuf = NULL;
                }
            }
            if (FD_ISSET(sockfd, &read_fds)) {
                nbytes = recv(sockfd, buf, sizeof(buf), 0);
                if(nbytes == 0){
                    close(sockfd);
                    return 0;
                }
                if (!o.sendonly)
                    Write(STDOUT_FILENO, buf, nbytes);
            }

            zmem(buf, sizeof(buf));
        }
    }

    return 0;
}

int ncat_listen()
{
    if (o.httpserver)
        return ncat_http_server();
    else if (o.udp)
        return ncat_listen_udp();
    else
        return ncat_listen_tcp();

    /* unreached */
    return 1;
}
