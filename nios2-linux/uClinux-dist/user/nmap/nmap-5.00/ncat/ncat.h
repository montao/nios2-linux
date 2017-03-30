/* Header file for Ncat */

/* $Id: ncat.h 13898 2009-06-25 00:23:07Z fyodor $ */

#ifndef NCAT_H_
#define NCAT_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <nbase.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

#include "nsock.h"
#include "util.h"
#include "sys_wrap.h"

#include "ncat_broker.h"
#include "ncat_connect.h"
#include "ncat_core.h"
#include "ncat_exec.h"
#include "ncat_hostmatch.h"
#include "ncat_listen.h"
#include "ncat_proxy.h"
#include "ncat_ssl.h"

/* Ncat information for output, etc. */
#define NCAT_NAME "Ncat"
#define NCAT_URL "http://nmap.org/ncat"
#define NCAT_VERSION "5.00"

/* structs */
struct conn_state {
    nsock_iod sock_nsi;
    nsock_iod stdin_nsi;
    nsock_ssl_session ssl_session;
    nsock_event_id latest_readsockev;
    nsock_event_id latest_readstdinev;
    nsock_event_id latest_writesockev;
};

struct socks4_data {
    char version;
    char type;
    unsigned short port;
    unsigned long address;
    char username[256];
};

/* defines */

/* Client-mode timeout for reads, infinite */
#define DEFAULT_READ_TIMEOUT -1

/* Client-mode timeout for writes, in msecs */
#define DEFAULT_WRITE_TIMEOUT 2000

/* Client-mode timeout for connection establishment, in msecs */
#define DEFAULT_CONNECT_TIMEOUT 10000

/* The default length of Ncat buffers */
#define DEFAULT_BUF_LEN      (1024)
#define DEFAULT_TCP_BUF_LEN  (1024 * 8)
#define DEFAULT_UDP_BUF_LEN  (1024 * 128)

/* Default Ncat port */
#define DEFAULT_NCAT_PORT 31337

/* Default port for SOCKS4 */
#define DEFAULT_SOCKS4_PORT 1080

/* The default port Ncat will connect to when trying to connect to an HTTP
 * proxy server.  The current setting is the default for squid and probably
 * other HTTP proxies. But it may also be 8080, 8888, etc.
 */
#define DEFAULT_PROXY_PORT 3128

/* Listen() backlog */
#define BACKLOG 10

/* The default maximum number of simultaneous connections Ncat will accept to
 * a listening port. You may want to increase or decrease this value depending
 * on your specific needs.
 */
#define DEFAULT_MAX_CONNS 100

/* SOCKS4 protocol responses */
#define SOCKS4_VERSION          4
#define SOCKS_CONNECT           1
#define SOCKS_BIND              2
#define SOCKS_CONN_ACC          90 /* woot */
#define SOCKS_CONN_REF          91
#define SOCKS_CONN_IDENT        92
#define SOCKS_CONN_IDENTDIFF    93

/* Length of IPv6 address */
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

/* Dummy WNOHANG for Windows */
#ifndef WNOHANG
#define WNOHANG 0
#endif

#endif
