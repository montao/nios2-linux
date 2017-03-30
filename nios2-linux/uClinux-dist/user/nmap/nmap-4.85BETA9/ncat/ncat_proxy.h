/* $Id: ncat_proxy.h 12981 2009-04-16 21:31:55Z david $ */

/*
 * Simple forking HTTP proxy.
 */
extern int ncat_http_server(void);

/*
 * Return an HTTP/1.1 CONNECT proxy request to send to an HTTP proxy server. If
 * proxy_auth is NULL, HTTP Proxy-Authorization headers are not included in the
 * request.
 */
extern char *http_proxy_client_request(char *proxy_auth);

/*
 * Handle SOCKS4 CD field error reporting. Return the error message to be used
 * in the final Ncat output. (It's final because these are all fatal errors.)
 *
 * See: http://archive.socks.permeo.com/protocol/socks4.protocol
 *
 * These error messages are taken verbatim from socks4.protocol (above)
 */
extern char *socks4_error(char cd);
