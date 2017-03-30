/* $Id: ncat_core.c 13656 2009-06-10 16:11:07Z david $ */

#include "ncat.h"
#include "util.h"
#include "sys_wrap.h"

#ifndef WIN32
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

struct sockaddr_storage srcaddr;
size_t srcaddrlen;
struct sockaddr_storage targetss;
size_t targetsslen;

struct sockaddr_storage httpconnect;
struct sockaddr_storage socksconnect;

/* Global options structure. */
struct options o;

/* Initializes global options to their default values. */
void options_init(void) {
    o.verbose = 0;
    o.debug = 0;
    o.target = NULL;
    o.af = AF_INET;
    o.broker = 0;
    o.listen = 0;
    o.keepopen = 0;
    o.sendonly = 0;
    o.recvonly = 0;
    o.telnet = 0;
    o.udp = 0;
    o.linedelay = 0;
    o.chat = 0;
    o.nodns = 0;
    o.normlogfd = -1;
    o.hexlogfd = -1;
    o.idletimeout = 0;
    o.crlf = 0;
    o.allow = 0;
    o.deny = 0;
    addrset_init(&o.allowset);
    addrset_init(&o.denyset);
    o.httpserver = 0;

    o.numsrcrtes = 0;
    o.srcrteptr = 4;

    o.conn_limit = -1;  /* Unset. */
    o.conntimeout = DEFAULT_CONNECT_TIMEOUT;

    o.cmdexec = NULL;
    o.shellexec = 0;
    o.proxy_auth = NULL;
    o.proxytype = NULL;

#ifdef HAVE_OPENSSL
    o.ssl = 0;
    o.sslcert = NULL;
    o.sslkey = NULL;
    o.sslverify = 0;
    o.ssltrustfile = NULL;
#endif
}

/* Tries to resolve the given name (or literal IP) into a sockaddr structure.
   Pass 0 for the port if you don't care. Returns 0 if hostname cannot be
   resolved. */
int resolve(char *hostname, unsigned short port,
            struct sockaddr_storage *ss, size_t *sslen, int af)
{
  struct addrinfo hints;
  struct addrinfo *result;
  char portbuf[16];
  int rc;

  assert(ss);
  assert(sslen);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = af;
  hints.ai_socktype = SOCK_DGRAM;
  if (o.nodns)
     hints.ai_flags |= AI_NUMERICHOST;

  /* Make the port number a string to give to getaddrinfo. */
  rc = Snprintf(portbuf, sizeof(portbuf), "%hu", port);
  assert(rc >= 0 && rc < sizeof(portbuf));

  rc = getaddrinfo(hostname, portbuf, &hints, &result);
  if (rc != 0 || result == NULL)
      return 0;
  assert(result->ai_addrlen > 0 && result->ai_addrlen <= (int) sizeof(struct sockaddr_storage));
  *sslen = result->ai_addrlen;
  memcpy(ss, result->ai_addr, *sslen);
  freeaddrinfo(result);
  return 1;
}

/* Broadcast a message to all the descriptors in fds. Returns -1 if any of the
   sends failed. */
int broadcast(const fd_set *fds, const fd_list_t *fdlist, const char *msg, size_t size)
{
    struct fdinfo *fdn;
    int i, ret;

    if (o.recvonly)
        return 0;

    ret = 0;
    for (i = 0; i <= fdlist->fdmax; i++) {
        if (!FD_ISSET(i, fds))
            continue;

        fdn = get_fdinfo(fdlist, i);
#ifdef HAVE_OPENSSL
        if (o.ssl && fdn->ssl != NULL) {
            if (SSL_write(fdn->ssl, msg, size) <= 0)
                ret = -1;
        } else
#endif
        {
            if (send(i, msg, size, 0) == -1) {
                if (o.debug > 1)
                    logdebug("Error sending to fd %d: %s.\n", i, socket_strerror(socket_errno()));
                ret = -1;
            }
        }
    }

    ncat_log_send(msg, size);

    return ret;
}

/* Do telnet WILL/WONT DO/DONT negotiations */
void dotelnet(int s, unsigned char *buf, size_t bufsiz)
{
    unsigned char *end = buf + bufsiz, *p;
    unsigned char tbuf[3];

    for (p = buf; buf < end; p++) {
        if (*p != 255) /* IAC */
            break;

        tbuf[0] = *p++;

        /* Answer DONT for WILL or WONT */
        if (*p == 251 || *p == 252)
            tbuf[1] = 254;

        /* Answer WONT for DO or DONT */
        else if (*p == 253 || *p == 254)
            tbuf[1] = 252;

        tbuf[2] = *++p;

        send(s, (const char *) tbuf, 3, 0);
    }
}

/* Return 1 if user is root, otherwise 0. */
int ncat_checkuid()
{
#ifdef WIN32
    return 1;
#else
    return (getuid() == 0 || geteuid() == 0);
#endif
}

/* sleep(), usleep(), msleep(), Sleep() -- all together now, "portability".
 *
 * There is no upper or lower limit to the delayval, so if you pass in a short
 * length of time <100ms, then you're likely going to get odd results.
 * This is because the Linux timeslice is 10ms-200ms. So don't expect
 * it to return for atleast that long.
 *
 * Block until the specified time has elapsed, then return 1.
 */
int ncat_delay_timer(int delayval)
{
    struct timeval s;

    s.tv_sec = delayval / 1000;
    s.tv_usec = (delayval % 1000) * (long) 1000;

    select(0, NULL, NULL, NULL, &s);
    return 1;
}

/* Open a logfile for writing.
 * Return the open file descriptor. */
int ncat_openlog(char *logfile)
{
    return Open(logfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
}

static int ncat_hexdump(int logfd, const char *data, int len);

void ncat_log_send(const char *data, size_t len)
{
    if (o.normlogfd != -1)
        Write(o.normlogfd, data, len);

    if (o.hexlogfd != -1)
        ncat_hexdump(o.hexlogfd, data, len);
}

void ncat_log_recv(const char *data, size_t len)
{
    /* Currently the log formats don't distinguish sends and receives. */
    ncat_log_send(data, len);
}

/* Convert session data to a neat hexdump logfile */
static int ncat_hexdump(int logfd, const char *data, int len)
{
    const char *p = data;
    char c;
    int i;
    char bytestr[4] = { 0 };
    char addrstr[10] = { 0 };
    char hexstr[16 * 3 + 5] = { 0 };
    char charstr[16 * 1 + 5] = { 0 };
    char outstr[80] = { 0 };

    /* FIXME: needs to be audited closer */
    for (i = 1; i <= len; i++) {
        if (i % 16 == 1) {
            /* Hex address output */
            Snprintf(addrstr, sizeof(addrstr), "%.4x", (u_int)(p - data));
        }

        c = *p;

        /* If the character isn't printable. Control characters, etc. */
        if (isprint(c) == 0)
            c = '.';

        /* hex for output */
        Snprintf(bytestr, sizeof(bytestr), "%02X ", (unsigned char) *p);
        strncat(hexstr, bytestr, sizeof(hexstr) - strlen(hexstr) - 1);

        /* char for output */
        Snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr) - strlen(charstr) - 1);

        if (i % 16 == 0) {
            /* neatly formatted output */
            Snprintf(outstr, sizeof(outstr), "[%4.4s]   %-50.50s  %s\n",
                     addrstr, hexstr, charstr);

            Write(logfd, outstr, strlen(outstr));
            zmem(outstr, sizeof(outstr));

            hexstr[0] = 0;
            charstr[0] = 0;
        } else if (i % 8 == 0) {
            /* cat whitespaces where necessary */
            strncat(hexstr, "  ", sizeof(hexstr) - strlen(hexstr) - 1);
            strncat(charstr, " ", sizeof(charstr) - strlen(charstr) - 1);
        }

        /* get the next byte */
        p++;
    }

    /* if there's still data left in the buffer, print it */
    if (strlen(hexstr) > 0) {
        Snprintf(outstr, sizeof(outstr), "[%4.4s]   %-50.50s  %s\n",
                    addrstr, hexstr, charstr);

        Write(logfd, outstr, strlen(outstr));
        zmem(outstr, sizeof(outstr));
    }

    return 1;
}
