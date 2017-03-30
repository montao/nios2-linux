/* $Id: ncat_broker.c 13656 2009-06-10 16:11:07Z david $ */

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
static void read_and_broadcast(int recv_socket);
static int chat_announce_connect(int fd, const struct sockaddr_storage *ss);
static int chat_announce_disconnect(int fd);
static char *chat_filter(char *buf, size_t size, int fd, int *nwritten);

int ncat_broker(void)
{
#ifdef HAVE_OPENSSL
    SSL_CTX *ctx;
#endif

    /* clear out structs */
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    zmem(&fdlist, sizeof(fdlist));

#ifndef WIN32
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

        if (o.debug > 1)
            logdebug("Broker connection count is %d\n", conn_count);

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
            } else {
                /* Handle incoming client data and distribute it. */
                read_and_broadcast(i);
            }

            fds_ready--;
        }
    }

    return 0;
}

/* Accept a connection on a listening socket. Allow or deny the connection.
   If allowed, add the new socket to the watch set. */
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

    if (o.verbose) {
        if (o.chat)
            loguser("Connection from %s on file descriptor %d.\n", inet_socktop(&remoteaddr), fd);
        else
            loguser("Connection from %s.\n", inet_socktop(&remoteaddr));
    }

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

    /* add to our lists */
    FD_SET(fd, &master);
    /* Now that a client is connected, pay attention to stdin. */
    if (!stdin_eof)
        FD_SET(STDIN_FILENO, &master);
    /* add it to our list of fds for maintaining maxfd */
    if (add_fd(&fdlist, fd, tmpssl) < 0)
         bye("add_fd() failed.");

    if (o.chat)
        chat_announce_connect(fd, &remoteaddr);
}

/* Read from recv_fd and broadcast whatever is read to all other descriptors in
   master, with the exception of stdin, listen_socket, and recv_fd itself.
   Handles EOL translation and chat mode. On read error or end of stream,
   closes the socket and removes it from the master list. */
static void read_and_broadcast(int recv_fd)
{
    char buf[DEFAULT_TCP_BUF_LEN];
    char *chatbuf, *outbuf;
    char *tempbuf = NULL;
    struct fdinfo *fdn = get_fdinfo(&fdlist, recv_fd);
    ssize_t nbytes;
    fd_set fds;

    assert(fdn);

#ifdef HAVE_OPENSSL
readagain:
    if (o.ssl && fdn->ssl)
        nbytes = SSL_read(fdn->ssl, buf, sizeof(buf));
    else
#endif
    if (recv_fd == STDIN_FILENO) {
        /* Behavior differs depending on whether this is stdin or a socket. */
        nbytes = read(recv_fd, buf, sizeof(buf));
        if (nbytes <= 0) {
            if (nbytes < 0 && o.verbose)
                logdebug("Error reading from stdin: %s\n", strerror(errno));
            if (nbytes == 0 && o.debug)
                logdebug("EOF on stdin\n");

            /* Don't close the file because that allows a socket to be fd 0. */
            FD_CLR(recv_fd, &master);
            /* But mark that we've seen EOF so it doesn't get re-added to the
               select list. */
            stdin_eof = 1;

            return;
        }

        if (o.crlf)
            fix_line_endings((char *) buf, &nbytes, &tempbuf);
    } else {
        /* From a connected socket, not stdin. */
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

            if (o.chat)
                chat_announce_disconnect(recv_fd);

            return;
        }

        /* If we received from something other than stdin, and --send-only was
           given, do no further processing. */
        if (o.sendonly)
            return;
    }

    if (o.debug > 1)
        logdebug("Handling data from client %d.\n", recv_fd);

    chatbuf = NULL;
    /* tempbuf is in use if we read from STDIN and fixed EOL */
    if (tempbuf == NULL)
        outbuf = buf;
    else
        outbuf = tempbuf;

    if (o.chat) {
        chatbuf = chat_filter(outbuf, nbytes, recv_fd, &nbytes);
        if (chatbuf == NULL) {
            if (o.verbose)
                logdebug("Error formatting chat message from fd %d\n", recv_fd);
        } else {
            outbuf = chatbuf;
        }
    }

    /* Write to everything in the master set, except the listener, sender, and
       stdin. */
    fds = master;
    FD_CLR(STDIN_FILENO, &fds);
    FD_CLR(listen_socket, &fds);
    FD_CLR(recv_fd, &fds);
    broadcast(&fds, &fdlist, outbuf, nbytes);

    free(chatbuf);
    free(tempbuf);
    tempbuf = NULL;

#ifdef HAVE_OPENSSL
    /* SSL can buffer our input, so doing another select()
     * won't necessarily work for us.  We jump back up to
     * read any more data we can grab now
     */
    if (o.ssl && fdn->ssl && SSL_pending(fdn->ssl))
        goto readagain;
#endif
}

/* Announce the new connection and who is already connected. */
static int chat_announce_connect(int fd, const struct sockaddr_storage *ss)
{
    char *buf = NULL;
    size_t size = 0, offset = 0;
    fd_set fds;
    int i, count, ret;

    strbuf_sprintf(&buf, &size, &offset,
        "<announce> %s is connected as <user%d>.\n", inet_socktop(ss), fd);

    fds = master;
    FD_CLR(STDIN_FILENO, &fds);
    FD_CLR(listen_socket, &fds);

    strbuf_sprintf(&buf, &size, &offset, "<announce> already connected: ");
    count = 0;
    for (i = 0; i < fdlist.fdmax; i++) {
        struct sockaddr_storage ss;
        socklen_t len = sizeof(ss);

        if (i == fd || !FD_ISSET(i, &fds))
            continue;

        if (getpeername(i, (struct sockaddr *) &ss, &len) == -1)
            bye("getpeername for sd %d failed: %s.", strerror(errno));

        if (count > 0)
            strbuf_sprintf(&buf, &size, &offset, ", ");

        strbuf_sprintf(&buf, &size, &offset, "%s as <user%d>", inet_socktop(&ss), i);

        count++;
    }
    if (count == 0)
        strbuf_sprintf(&buf, &size, &offset, "nobody");
    strbuf_sprintf(&buf, &size, &offset, ".\n");

    ret = broadcast(&fds, &fdlist, buf, offset);

    free(buf);

    return ret;
}

static int chat_announce_disconnect(int fd)
{
    fd_set fds;
    char buf[128];
    int n;

    n = Snprintf(buf, sizeof(buf),
        "<announce> <user%d> is disconnected.\n", fd);
    if (n >= sizeof(buf) || n < 0)
        return -1;

    fds = master;
    FD_CLR(STDIN_FILENO, &fds);
    FD_CLR(listen_socket, &fds);

    return broadcast(&fds, &fdlist, buf, n);
}

/*
 * This is stupid. But it's just a bit of fun.
 *
 * The file descriptor of the sender is prepended to the
 * message sent to clients, so you can distinguish
 * each other with a degree of sanity. This gives a
 * similar effect to an IRC session. But stupider.
 */
static char *chat_filter(char *buf, size_t size, int fd, int *nwritten)
{
    char *result = NULL;
    size_t n = 0;
    const char *p;
    int i;

    n = 32;
    result = (char *) safe_malloc(n);
    i = Snprintf(result, n, "<user%d> ", fd);

    /* Escape control characters. */
    for (p = buf; p - buf < size; p++) {
        char repl[32];
        int repl_len;

        if (isprint(*p) || *p == '\r' || *p == '\n' || *p == '\t') {
            repl[0] = *p;
            repl_len = 1;
        } else {
            repl_len = Snprintf(repl, sizeof(repl), "\\%03o", (unsigned char) *p);
        }

        if (i + repl_len > n) {
            n = (i + repl_len) * 2;
            result = (char *) safe_realloc(result, n + 1);
        }
        memcpy(result + i, repl, repl_len);
        i += repl_len;
    }
    /* Trim to length. (Also does initial allocation when str is empty.) */
    result = (char *) safe_realloc(result, i + 1);
    result[i] = '\0';

    *nwritten = i;

    return result;
}
