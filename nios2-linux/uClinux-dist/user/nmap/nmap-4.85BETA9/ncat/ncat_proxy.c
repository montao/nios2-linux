/* $Id: ncat_proxy.c 13231 2009-05-09 03:24:52Z david $ */

#include "base64.h"
#include "http.h"
#include "nsock.h"
#include "ncat.h"
#include "sys_wrap.h"

#ifndef WIN32
#include <unistd.h>
#endif

/* SIG_CHLD handler */
static void proxyreaper(int signo)
{
    while (Waitpid(-1, NULL, WNOHANG) > 0);
}

/* send a '\0'-terminated string. */
static int send_string(int sock, const char *s)
{
    return send(sock, s, strlen(s), 0);
}

static void http_server_handler(int c);
static char *http_code2str(int code);

static void fork_handler(int s, int c);

static int handle_connect(struct socket_buffer *client_sock,
    struct http_request *request);
static int handle_method(struct socket_buffer *client_sock,
    struct http_request *request);

/*
 * Simple forking HTTP proxy. It is an HTTP/1.0 proxy with knowledge of
 * HTTP/1.1. (The things lacking for HTTP/1.1 are the chunked transfer encoding
 * and the expect mechanism.) The proxy supports the CONNECT, GET, HEAD, and
 * POST methods. It supports Basic authentication of clients (use the
 * --proxy-auth option).
 *
 * HTTP/1.1 is defined in RFC 2616. Many comments refer to that document.
 * http://tools.ietf.org/html/rfc2616
 *
 * HTTP authentication is discussed in RFC 2617.
 * http://tools.ietf.org/html/rfc2617
 *
 * The CONNECT method is documented in an Internet draft and is specified as the
 * way to proxy HTTPS in RFC 2817, section 5.
 * http://tools.ietf.org/html/draft-luotonen-web-proxy-tunneling-01
 * http://tools.ietf.org/html/rfc2817#section-5
 *
 * The CONNECT method is not limited to HTTP, but is potentially capable of
 * connecting to any TCP port on any host. The proxy connection is requested
 * with an HTTP request, but after that, the proxy does no interpretation of the
 * data passing through it. See section 6 of the above mentioned draft for the
 * security implications.
 */
int ncat_http_server(void)
{
    int c, s;
    size_t sslen;
    struct sockaddr_storage conn;

#ifndef WIN32
    Signal(SIGCHLD, proxyreaper);
#endif

    s = do_listen(SOCK_STREAM);

    for (;;) {
        sslen = sizeof(conn);

        c = accept(s, (struct sockaddr *) &conn, (socklen_t *) &sslen);

        if (c == -1) {
            if (errno == EINTR)
                continue;
            die("accept");
        }

        if (!allow_access(&conn)) {
            Close(c);
            continue;
        }

        fork_handler(s, c);
    }

    return 0;
}

#ifdef WIN32
/* On Windows we don't actually fork but rather start a thread. */

static DWORD WINAPI handler_thread_func(void *data)
{
    http_server_handler(*((int *) data));
    free(data);

    return 0;
}

static void fork_handler(int s, int c)
{
    int *data;
    HANDLE thread;

    data = (int *) malloc(sizeof(int));
    *data = c;
    thread = CreateThread(NULL, 0, handler_thread_func, data, 0, NULL);
    if (thread == NULL) {
        if (o.verbose)
            logdebug("Error in CreateThread: %d\n", GetLastError());
        free(data);
        return;
    }
    CloseHandle(thread);
}
#else
static void fork_handler(int s, int c)
{
    int rc;

    rc = fork();
    if (rc == -1) {
        return;
    } else if (rc == 0) {
        Close(s);

        if (!o.debug) {
            Close(STDIN_FILENO);
            Close(STDOUT_FILENO);
            Close(STDERR_FILENO);
        }

        http_server_handler(c);
        exit(0);
    } else {
        Close(c);
    }
}
#endif

/* Is this one of the methods we can handle? */
static int method_is_known(const char *method)
{
    return strcmp(method, "CONNECT") == 0
        || strcmp(method, "GET") == 0
        || strcmp(method, "HEAD") == 0
        || strcmp(method, "POST") == 0;
}

static void http_server_handler(int c)
{
    int code;
    struct socket_buffer sock;
    struct http_request request;
    char *buf;

    socket_buffer_init(&sock, c);

    code = http_read_request_line(&sock, &buf);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error reading Request-Line.\n");
        send_string(c, http_code2str(code));
        Close(c);
        return;
    }
    if (o.debug > 1)
        logdebug("Request-Line: %s", buf);
    code = http_parse_request_line(buf, &request);
    free(buf);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error parsing Request-Line.\n");
        send_string(c, http_code2str(code));
        Close(c);
        return;
    }

    if (!method_is_known(request.method)) {
        if (o.debug > 1)
            logdebug("Bad method: %s.\n", request.method);
        http_request_free(&request);
        send_string(c, http_code2str(405));
        Close(c);
        return;
    }

    code = http_read_header(&sock, &buf);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error reading header.\n");
        http_request_free(&request);
        send_string(c, http_code2str(code));
        Close(c);
        return;
    }
    if (o.debug > 1)
        logdebug("Header:\n%s", buf);
    code = http_request_parse_header(&request, buf);
    free(buf);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error parsing header.\n");
        http_request_free(&request);
        send_string(c, http_code2str(code));
        Close(c);
        return;
    }

    /* Check authentication. */
    if (o.proxy_auth) {
        char *auth;
        int ret;

        auth = http_header_get_first(request.header, "Proxy-Authorization");
        if (auth == NULL) {
            /* No authentication sent. */
            http_request_free(&request);
            send_string(c, http_code2str(407));
            Close(c);
            return;
        }

        ret = http_check_auth_basic(o.proxy_auth, auth);
        free(auth);
        if (!ret) {
            /* Password doesn't match. */
            http_request_free(&request);
            /* RFC 2617, section 1.2: "If a proxy does not accept the
               credentials sent with a request, it SHOULD return a 407 (Proxy
               Authentication Required). */
            send_string(c, http_code2str(407));
            Close(c);
            return;
        }
    }

    if (strcmp(request.method, "CONNECT") == 0) {
        code = handle_connect(&sock, &request);
    } else if (strcmp(request.method, "GET") == 0
        || strcmp(request.method, "HEAD") == 0
        || strcmp(request.method, "POST") == 0) {
        code = handle_method(&sock, &request);
    } else {
        code = 500;
    }
    http_request_free(&request);

    if (code != 0) {
        send_string(c, http_code2str(code));
        Close(c);
        return;
    }

    Close(c);
}

static int handle_connect(struct socket_buffer *client_sock,
    struct http_request *request)
{
    struct sockaddr_storage ss;
    size_t sslen = sizeof(ss);
    int maxfd, s;
    char *line;
    size_t len;
    fd_set m, r;

    if (request->uri.port == -1) {
        if (o.verbose)
            logdebug("No port number in CONNECT URI.\n");
        return 400;
    }
    if (o.debug > 1)
        logdebug("CONNECT to %s:%hu.\n", request->uri.host, request->uri.port);

    if (!resolve(request->uri.host, request->uri.port, &ss, &sslen, AF_UNSPEC)) {
        if (o.debug)
            logdebug("Can't resolve name %s.\n", request->uri.host);
        return 504;
    }

    s = Socket(ss.ss_family, SOCK_STREAM, IPPROTO_TCP);

    if (connect(s, (struct sockaddr *) &ss, sslen) == -1) {
        if (o.debug)
            logdebug("Can't connect to %s.\n", inet_socktop(&ss));
        Close(s);
        return 504;
    }

    send_string(client_sock->sd, http_code2str(200));

    /* Clear out whatever is left in the socket buffer. The client may have
       already sent the first part of its request to the origin server. */
    line = socket_buffer_remainder(client_sock, &len);
    if (send(s, line, len, 0) < 0) {
        if (o.debug)
            logdebug("Error sending %u leftover bytes: %s.\n", len, strerror(errno));
        Close(s);
        return 0;
    }

    maxfd = client_sock->sd < s ? s : client_sock->sd;
    FD_ZERO(&m);
    FD_SET(client_sock->sd, &m);
    FD_SET(s, &m);

    errno = 0;

    while (!errno || errno == EINTR) {
        char buf[DEFAULT_TCP_BUF_LEN];
        int len, numready;

        r = m;

        numready = Select(maxfd + 1, &r, NULL, NULL, NULL);

        zmem(buf, sizeof(buf));

        if (FD_ISSET(client_sock->sd, &r)) {
            if ((len = recv(client_sock->sd, buf, sizeof(buf), 0)) < 0)
                continue;

            if (!len)
                break;

            if (send(s, buf, len, 0) < 0)
                continue;
        }

        if (FD_ISSET(s, &r)) {
            if ((len = recv(s, buf, sizeof(buf), 0)) < 0)
                continue;

            if (!len)
                break;

            if (send(client_sock->sd, buf, len, 0) < 0)
                continue;
        }
    }

    close(s);

    return 0;
}

static int do_transaction(struct http_request *request,
    struct socket_buffer *client_sock, struct socket_buffer *server_sock);

/* Generic handler for GET, HEAD, and POST methods. */
static int handle_method(struct socket_buffer *client_sock,
    struct http_request *request)
{
    struct socket_buffer server_sock;
    struct sockaddr_storage ss;
    size_t sslen = sizeof(ss);
    int code;
    int s;

    if (strcmp(request->uri.scheme, "http") != 0) {
        if (o.verbose)
            logdebug("Unknown scheme in URI: %s.\n", request->uri.scheme);
        return 400;
    }
    if (request->uri.port == -1) {
        if (o.verbose)
            logdebug("Unknown port in URI.\n");
        return 400;
    }

    if (!resolve(request->uri.host, request->uri.port, &ss, &sslen, AF_UNSPEC)) {
        if (o.debug)
            logdebug("Can't resolve name %s:%d.\n", request->uri.host, request->uri.port);
        return 504;
    }

    /* RFC 2616, section 5.1.2: "In order to avoid request loops, a proxy MUST
       be able to recognize all of its server names, including any aliases,
       local variations, and the numeric IP address. */
    if (request->uri.port == o.portno && addr_is_local(&ss)) {
        if (o.verbose)
            logdebug("Proxy loop detected: %s:%d\n", request->uri.host, request->uri.port);
        return 403;
    }

    s = Socket(ss.ss_family, SOCK_STREAM, IPPROTO_TCP);

    if (connect(s, (struct sockaddr *) &ss, sslen) == -1) {
        if (o.debug)
            logdebug("Can't connect to %s.\n", inet_socktop(&ss));
        Close(s);
        return 504;
    }

    socket_buffer_init(&server_sock, s);

    code = do_transaction(request, client_sock, &server_sock);

    close(server_sock.sd);

    if (code != 0)
        return code;

    return 0;
}

/* Do a GET, HEAD, or POST transaction. */
static int do_transaction(struct http_request *request,
    struct socket_buffer *client_sock, struct socket_buffer *server_sock)
{
    char buf[BUFSIZ];
    struct http_response response;
    char *line;
    char *request_str, *response_str;
    size_t len;
    int code, n;

    /* We don't handle the chunked transfer encoding, which in the absence of a
       Content-Length is the only way we know the end of a request body. RFC
       2616, section 4.4 says, "If a request contains a message-body and a
       Content-Length is not given, the server SHOULD respond with 400 (bad
       request) if it cannot determine the length of the message, or with 411
       (length required) if it wishes to insist on receiving a valid
       Content-Length." */
    if (strcmp(request->method, "POST") == 0 && request->content_length == 0) {
        if (o.debug)
            logdebug("POST request with no Content-Length.\n");
        return 400;
    }

    /* The version we use to talk to the server. */
    request->version = HTTP_10;

    /* Remove headers that only apply to our connection with the client. */
    code = http_header_remove_hop_by_hop(&request->header);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error removing hop-by-hop headers.\n");
        return code;
    }

    /* Build the Host header. */
    if (request->uri.port == -1 || request->uri.port == 80)
        n = Snprintf(buf, sizeof(buf), "%s", request->uri.host);
    else
        n = Snprintf(buf, sizeof(buf), "%s:%hu", request->uri.host, request->uri.port);
    if (n < 0 || n >= sizeof(buf)) {
        /* Request Entity Too Large. */
        return 501;
    }
    request->header = http_header_set(request->header, "Host", buf);

    request->header = http_header_set(request->header, "Connection", "close");

    /* Send the request to the server. */
    request_str = http_request_to_string(request, &len);
    n = send(server_sock->sd, request_str, len, 0);
    free(request_str);
    if (n < 0)
        return 504;
    /* Send the request body, if any. Count up to Content-Length. */
    while (request->bytes_transferred < request->content_length) {
        n = socket_buffer_read(client_sock, buf, MIN(sizeof(buf), request->content_length - request->bytes_transferred));
        if (n < 0)
            return 504;
        if (n == 0)
            break;
        request->bytes_transferred += n;
        n = send(server_sock->sd, buf, n, 0);
        if (n < 0)
            return 504;
    }
    if (o.debug && request->bytes_transferred < request->content_length)
        logdebug("Received only %lu request body bytes (Content-Length was %lu).\n", request->bytes_transferred, request->content_length);


    /* Read the response. */
    code = http_read_status_line(server_sock, &line);
    if (o.debug > 1)
        logdebug("Status-Line: %s", line);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error reading Status-Line.\n");
        return 0;
    }
    code = http_parse_status_line(line, &response);
    free(line);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error parsing Status-Line.\n");
        return 0;
    }

    code = http_read_header(server_sock, &line);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error reading header.\n");
        return 0;
    }
    if (o.debug > 1)
        logdebug("Response header:\n%s", line);

    code = http_response_parse_header(&response, line);
    free(line);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error parsing response header.\n");
        return 0;
    }


    /* The version we use to talk to the client. */
    response.version = HTTP_10;

    /* Remove headers that only apply to our connection with the server. */
    code = http_header_remove_hop_by_hop(&response.header);
    if (code != 0) {
        if (o.verbose)
            logdebug("Error removing hop-by-hop headers.\n");
        return code;
    }

    response.header = http_header_set(response.header, "Connection", "close");

    /* Send the response to the client. */
    response_str = http_response_to_string(&response, &len);
    n = send(client_sock->sd, response_str, len, 0);
    free(response_str);
    if (n < 0) {
        http_response_free(&response);
        return 504;
    }
    /* If the Content-Length is 0, read until the connection is closed.
       Otherwise read until the Content-Length. At this point it's too late to
       return our own error code so return 0 in case of any error. */
    while (response.content_length == 0
        || response.bytes_transferred < response.content_length) {
        size_t remaining = response.content_length - response.bytes_transferred;
        size_t count;

        count = sizeof(buf);
        if (response.content_length > 0 && remaining < count)
            count = remaining;
        n = socket_buffer_read(server_sock, buf, count);
        if (n <= 0)
            break;
        response.bytes_transferred += n;
        n = send(client_sock->sd, buf, n, 0);
        if (n < 0)
            break;
    }

    http_response_free(&response);

    return 0;
}

static char *http_code2str(int code)
{
    /* See RFC 2616, section 6.1.1 for status codes. */
    switch (code) {
    case 200:
        return "HTTP/1.0 200 OK\r\n\r\n";
    case 400:
        return "HTTP/1.0 400 Bad Request\r\n\r\n";
    case 403:
        return "HTTP/1.0 403 Forbidden\r\n\r\n";
    case 405:
        /* RFC 2616, section 14.7 for Allow. */
        return "\
HTTP/1.0 405 Method Not Allowed\r\n\
Allow: CONNECT, GET, HEAD, POST\r\n\
\r\n";
    case 407:
        return "\
HTTP/1.0 407 Proxy Authentication Required\r\n\
Proxy-Authenticate: Basic realm=\"Ncat\"\r\n\
\r\n";
    case 413:
        return "HTTP/1.0 413 Request Entity Too Large\r\n\r\n";
    case 501:
        return "HTTP/1.0 501 Not Implemented\r\n\r\n";
    case 504:
        return "HTTP/1.0 504 Gateway Timeout\r\n\r\n";
    default:
        return "HTTP/1.0 500 Internal Server Error\r\n\r\n";
    }

    return NULL;
}

/*
 * Return an HTTP/1.1 CONNECT proxy request to send to an HTTP proxy server. If
 * proxy_auth is NULL, HTTP Proxy-Authorization headers are not included in the
 * request.
 */
char *http_proxy_client_request(char *proxy_auth)
{
    char *b64_auth;
    static char proxy_request[DEFAULT_BUF_LEN];
    int pos;
    const char *proxyhost;
    char hostbuf[INET6_ADDRSTRLEN + 9]; /* [ + address + ] + : + port + \0 */
    const char *host = inet_socktop(&httpconnect);
    unsigned short port = inet_port(&httpconnect);
    char *s6s = o.af == AF_INET6 ? "[" : "", *s6e = o.af == AF_INET6 ? "]" : "";

    if (port)
        Snprintf(hostbuf, sizeof hostbuf, "%s%s%s:%hu", s6s, host, s6e, port);
    else
        Snprintf(hostbuf, sizeof hostbuf, "%s%s%s", s6s, host, s6e);

    proxyhost = inet_socktop(&targetss);

    if (o.debug)
        logdebug("Proxy CONNECT target: %s\n", hostbuf);

    pos = Snprintf(proxy_request, sizeof(proxy_request),
                   "CONNECT %s HTTP/1.1\r\n", hostbuf);

    if (proxy_auth != NULL) {
        b64_auth = b64enc((unsigned char *) proxy_auth, strlen((char *)proxy_auth));

        if (o.debug)
            logdebug("Proxy auth (base64enc): %s\n", b64_auth);

        pos += Snprintf(proxy_request + pos, sizeof(proxy_request) - pos,
                        "Proxy-Authorization: Basic %s\r\n", b64_auth);

        free(b64_auth);
    }

    Snprintf(proxy_request + pos, sizeof(proxy_request) - pos,
             "Host: %s\r\n\r\n", proxyhost);

    return (proxy_request);
}

/*
 * Handle SOCKS4 CD field error reporting. Return the error message to be used
 * in the final Ncat output. (It's final because these are all fatal errors.)
 *
 * See: http://archive.socks.permeo.com/protocol/socks4.protocol
 *
 * These error messages are taken verbatim from socks4.protocol (above)
 */
char *socks4_error(char cd)
{
    switch (cd) {
        case SOCKS_CONN_REF:
            return "request rejected or failed";
            break;

        case SOCKS_CONN_IDENT:
            return "request rejected because SOCKS4 server cannot connect to identd";
            break;

        case SOCKS_CONN_IDENTDIFF:
            return "request rejected because SOCKS4 client and identd reported different userid's";
            break;

        default:
            return "Invalid SOCKS4 error code. Broken SOCKS4 implementation?";
    }
}
