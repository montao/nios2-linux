/* $Id: ncat_connect.c 13165 2009-05-04 07:04:53Z daniel $ */

#include "nsock.h"
#include "ncat.h"
#include "util.h"
#include "sys_wrap.h"

#include "nbase.h"

#ifndef WIN32
#include <unistd.h>
#include <netdb.h>
#endif
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

static void connect_evt_handler(nsock_pool nsp, nsock_event evt, void *mydata);

#ifdef HAVE_OPENSSL
/* This callback is called for every certificate in a chain. ok is true if
   OpenSSL's internal verification has verified the certificate. We don't change
   anything about the verification, we only need access to the certificates to
   provide diagnostics. */
static int verify_callback(int ok, X509_STORE_CTX *store)
{
    char digest_buf[SHA1_STRING_LENGTH + 1];
    X509 *cert = X509_STORE_CTX_get_current_cert(store);
    int err = X509_STORE_CTX_get_error(store);

    assert(ssl_cert_fp_str_sha1(cert, digest_buf, sizeof(digest_buf)) != NULL);

    if (ok) {
        if (o.debug)
            logdebug("Certificate OK.\n");
    } else {
        if (o.verbose) {
            loguser("Error: certificate verification failed (%s).\n",
                X509_verify_cert_error_string(err));
            loguser("SHA-1 fingerprint: %s\n", digest_buf);
        }
    }

    if (o.debug > 1) {
        loguser("Subject:\n");
        X509_NAME_print_ex_fp(stderr, X509_get_subject_name(cert), 2, XN_FLAG_MULTILINE);
        loguser("\n");
        loguser("Issuer:\n");
        X509_NAME_print_ex_fp(stderr, X509_get_issuer_name(cert), 2, XN_FLAG_MULTILINE);
        loguser("\n");
    }

    return ok;
}

static void set_ssl_ctx_options(SSL_CTX *ctx)
{
    if (o.sslverify) {
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);

        if (o.ssltrustfile == NULL) {
            ssl_load_default_ca_certs(ctx);
        } else {
            if (o.debug)
                logdebug("Using trusted CA certificates from %s.\n", o.ssltrustfile);
            if (SSL_CTX_load_verify_locations(ctx, o.ssltrustfile, NULL) != 1) {
                bye("Could not load trusted certificates from %s.\n%s",
                    o.ssltrustfile, ERR_error_string(ERR_get_error(), NULL));
            }
        }
    } else {
        if (o.ssl && o.debug)
            logdebug("Not doing certificate verification.\n");
    }
}
#endif

int ncat_connect(void) {
    struct conn_state cs;
    nsock_pool mypool;
    nsock_event_id ev;
    int rc;

    /* Create an nsock pool */
    if ((mypool = nsp_new(NULL)) == NULL)
        bye("Failed to create nsock_pool.");

    if (o.debug > 1)
        /* A trace level of 1 still gives you an awful lot. */
        nsp_settrace(mypool, 1, nsock_gettimeofday());

#ifdef HAVE_OPENSSL
    set_ssl_ctx_options((SSL_CTX *)nsp_ssl_init(mypool));
#endif

    /* create an iod for a new socket */
    if ((cs.sock_nsi = nsi_new(mypool, NULL)) == NULL)
        bye("Failed to create nsock_iod.");

    if (srcaddr.ss_family != AF_UNSPEC)
        nsi_set_localaddr(cs.sock_nsi, &srcaddr, srcaddrlen);

    if (o.numsrcrtes) {
        struct sockaddr_in *sin = (struct sockaddr_in *) &targetss;
        unsigned char *ipopts = NULL;
        size_t ipoptslen = 0;

        if (o.af != AF_INET)
            bye("Sorry, -g can only currently be used with IPv4.");
        ipopts = buildsrcrte(sin->sin_addr, o.srcrtes, o.numsrcrtes, o.srcrteptr, &ipoptslen);

        nsi_set_ipoptions(cs.sock_nsi, ipopts, ipoptslen);
        free(ipopts); /* Nsock has its own copy */
    }

    if (o.udp) {
        ev = nsock_connect_udp(mypool, cs.sock_nsi, connect_evt_handler,
                               &cs, (struct sockaddr *) &targetss, targetsslen,
                               inet_port(&targetss));
    }
#ifdef HAVE_OPENSSL
    else if (o.ssl) {
        cs.ssl_session = NULL;
        ev = nsock_connect_ssl(mypool, cs.sock_nsi, connect_evt_handler,
                               o.conntimeout, &cs,
                               (struct sockaddr *) &targetss, targetsslen,
                               inet_port(&targetss), cs.ssl_session);
    }
#endif
    else {
        ev = nsock_connect_tcp(mypool, cs.sock_nsi, connect_evt_handler,
                               o.conntimeout, &cs,
                               (struct sockaddr *) &targetss, targetsslen,
                               inet_port(&targetss));
    }

    /* connect */
    rc = nsock_loop(mypool, -1);
    nsp_delete(mypool);

    return rc;
}

/* handle nsock-powered connections */
static void connect_evt_handler(nsock_pool nsp, nsock_event evt, void *mydata)
{
    int nbytes = 100, read_timeout = DEFAULT_READ_TIMEOUT,
        write_timeout = DEFAULT_WRITE_TIMEOUT;
    static int is_socks4_connected;
    static int holdstdin;
    int mysock;
    char *buf = NULL;
    nsock_iod nsi = nse_iod(evt);

    enum nse_status status = nse_status(evt);
    enum nse_type type = nse_type(evt);
    struct sockaddr_storage peer;
    struct conn_state *cs;

    /* drop conn_evt data into our struct */
    cs = (struct conn_state *) mydata;

    /* debugging */
    if (o.debug > 1)
        logdebug("Received callback of type %s with status %s\n",
                 nse_type2str(type), nse_status2str(status));

    /* User-defined read/write timeouts */
    if (o.idletimeout)
            write_timeout = read_timeout = o.idletimeout;

    mysock = nsi_getsd(cs->sock_nsi);

    /* Handle nsock responses for the connection */
    if (status == NSE_STATUS_SUCCESS) {
            switch (type) {
            case NSE_TYPE_CONNECT:
            case NSE_TYPE_CONNECT_SSL:

                /* Get peer information */
                nsi_getlastcommunicationinfo(nsi, NULL, NULL, NULL,
                        (struct sockaddr *) &peer,
                        sizeof(struct sockaddr_storage));

#ifdef HAVE_OPENSSL
                if (nsi_checkssl(cs->sock_nsi)) {
                    /* Check the domain name. ssl_post_connect_check prints an
                       error message if appropriate. */
                    if (!ssl_post_connect_check((SSL *)nsi_getssl(cs->sock_nsi), o.target))
                        bye("Certificate verification error.");
                    if (cs->ssl_session) {
                        if (cs->ssl_session == (SSL_SESSION *)
                                (nsi_get0_ssl_session(cs->sock_nsi))) {
                            /* nothing required */
                        } else {
                            SSL_SESSION_free((SSL_SESSION *) cs->ssl_session);
                            cs->ssl_session = (SSL_SESSION *)
                                (nsi_get1_ssl_session(cs->sock_nsi));
                        }
                    } else {
                        cs->ssl_session = (SSL_SESSION *)
                            (nsi_get1_ssl_session(cs->sock_nsi));
                    }
                }
#endif

                if (o.verbose)
                    loguser("Connected to %s.\n", inet_socktop(&peer));

                /* Create IOD for nsp->stdin */
                if ((cs->stdin_nsi = nsi_new2(nsp, 0, NULL)) == NULL)
                    bye("Failed to create stdin nsiod.");

                /* command to execute */
                if (o.cmdexec)
                    netexec(mysock, o.cmdexec);

                /* format an http proxy request */
                if (httpconnect.ss_family != AF_UNSPEC) {
                    static char *proxy_request;

                    proxy_request = http_proxy_client_request(o.proxy_auth);

                    cs->latest_writesockev =
                        nsock_write(nsp, cs->sock_nsi, connect_evt_handler,
                                    write_timeout, cs, proxy_request, -1);
                }

                /* we're doing SOCKS proxying */
                if (socksconnect.ss_family != AF_UNSPEC) {
                    struct sockaddr_in *sin = (struct sockaddr_in *) &socksconnect;
                    struct socks4_data socks4msg;

                    /* Fill the socks4_data struct */
                    zmem(&socks4msg, sizeof(socks4msg));
                    socks4msg.version = SOCKS4_VERSION;
                    socks4msg.type = SOCKS_CONNECT;
                    socks4msg.port = sin->sin_port;
                    socks4msg.address = sin->sin_addr.s_addr;
                    if (o.proxy_auth)
                        Strncpy(socks4msg.username, (char *) o.proxy_auth, sizeof(socks4msg.username));

                    cs->latest_writesockev =
                        nsock_write(nsp, cs->sock_nsi, connect_evt_handler,
                                    write_timeout, cs, (char *) &socks4msg,
                                    8 + strlen(socks4msg.username) + 1);

                    cs->latest_readsockev =
                        nsock_readbytes(nsp, cs->sock_nsi, connect_evt_handler,
                                        read_timeout, cs, 8);

                    /* We don't want to read from stdin and send the data
                     * until we're actually connected to the proxy
                     */
                    holdstdin++;
                }

                if (o.sendonly == 0) {
                    /* socket event? */
                    cs->latest_readsockev =
                        nsock_read(nsp, cs->sock_nsi, connect_evt_handler,
                                   read_timeout, cs);
                }

                if (o.recvonly == 0 && holdstdin == 0) {
                    /* stdin-fd event? */
                    cs->latest_readstdinev =
                        nsock_readbytes(nsp, cs->stdin_nsi,
                                        connect_evt_handler, read_timeout, cs, 0);
                }

                break;

            case NSE_TYPE_READ:
                /* read buffer */
                buf = nse_readbuf(evt, &nbytes);

                /* READ from socket */
                if (nsi == cs->sock_nsi) {
                    if (o.linedelay)
                        ncat_delay_timer(o.linedelay);

                    if (o.sendonly == 0) {
                        if (socksconnect.ss_family != AF_UNSPEC && is_socks4_connected == 0) {

                            if (nbytes < 7)
                                bye("Connection to SOCKS4 proxy failed: Invalid SOCKS4 response.");

                            /* validate reply status code */
                            if(buf[1] != 90)
                                bye("Connection to SOCKS4 proxy failed: %s.", socks4_error(buf[1]));

                            /* and print rest of data to stdout */
                            Write(STDOUT_FILENO, buf + 8, nbytes - 8);

                            /*
                             * Once we've connected to the socks4 server
                             * we don't need to send any more SOCKS4 requests
                             * for the rest of this session.
                             * Therefore, we don't need to hit this code again.
                             */
                            is_socks4_connected = 1;

                            /* Go back to reading data from the socket */
                            cs->latest_readsockev =
                                nsock_read(nsp, cs->sock_nsi,
                                           connect_evt_handler,
                                           read_timeout, cs);

                            /* Now we can do stdin->net data */
                            if (o.recvonly == 0 && --holdstdin == 0)
                                cs->latest_readstdinev =
                                        nsock_readbytes(nsp, cs->stdin_nsi,
                                              connect_evt_handler, read_timeout, cs, 0);
                        } else {
                            if (o.telnet)
                                dotelnet(mysock, (unsigned char *) buf, nbytes);

                            /* Write socket data to stdout */
                            Write(STDOUT_FILENO, buf, nbytes);
                            ncat_log_recv(buf, nbytes);

                            cs->latest_readsockev =
                                nsock_readbytes(nsp, cs->sock_nsi,
                                           connect_evt_handler,
                                           read_timeout, cs, 0);
                        }
                    }
                } else {
                    /* read from stdin */
                    if (o.linedelay)
                        ncat_delay_timer(o.linedelay);

                    if (o.recvonly == 0) {
                        char *tmp = NULL;

                        if (o.crlf && buf[nbytes - 1] == '\n' && buf[nbytes - 2] != '\r') {
                            tmp = (char *) safe_malloc(nbytes + 1);
                            memcpy(tmp, buf, nbytes - 1);
                            memcpy(tmp + nbytes - 1, "\r\n", 2);
                            buf = tmp;
                            nbytes++;
                        }

                        nsock_write(nsp, cs->sock_nsi, connect_evt_handler,
                                write_timeout, cs, buf, nbytes);
                        ncat_log_send(buf, nbytes);

                        if (tmp)
                            free(tmp); /* buf */
                    }

                }

                break;

            case NSE_TYPE_WRITE:
                if (nsi == cs->sock_nsi) {
                    /* The write to the socket was successful. Allow reading
                       more from stdin now. */
                    cs->latest_readstdinev =
                        nsock_readbytes(nsp, cs->stdin_nsi,
                                connect_evt_handler, read_timeout, cs, 0);
                }
                break;

            case NSE_TYPE_TIMER:
                break;

            default:
                bye("connect_evt_handler got bogus type.");
                break;
        }   /* end switch */

    } else if (status == NSE_STATUS_EOF) {
        /* Close up if we either got EOF from network side (e.g. the
         * TCP connection closed on remote side), or if we got EOF
         * on stdin while using --send-only mode. */
        if (nsi == cs->sock_nsi || o.sendonly) {
            nsi_delete(cs->stdin_nsi, NSOCK_PENDING_NOTIFY);
            nsi_delete(cs->sock_nsi, NSOCK_PENDING_NOTIFY);
        }
    } else if (status == NSE_STATUS_ERROR) {
        if (socket_errno() == EINPROGRESS)
            /* XXX: this is weird. errno always seems to be incorrect on ret from nsock. */
            errno = ECONNREFUSED;
        if (socket_errno() == EINTR) {
            sleep(1);
        } else if (socket_errno() != 0) {
            if (o.verbose)
                loguser("%s.\n", socket_strerror(socket_errno()));
            exit(1);
        }
    } else if (status == NSE_STATUS_TIMEOUT){
            errno = ETIMEDOUT;
            if (o.verbose)
                loguser("%s.\n", socket_strerror(socket_errno()));
            exit(1);
    }

    return;
}
