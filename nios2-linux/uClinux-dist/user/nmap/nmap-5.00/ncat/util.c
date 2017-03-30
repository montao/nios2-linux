/* $Id: util.c 13631 2009-06-09 01:08:45Z jah $ */

#include "sys_wrap.h"
#include "util.h"
#include "ncat.h"

#include <assert.h>
#include <stdio.h>
#ifdef WIN32
#include <iphlpapi.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

/* safely add 2 size_t */
size_t sadd(size_t l, size_t r)
{
    size_t  t;

    t = l + r;
    if(t < l)
        bye("integer overflow %lu + %lu.", (u_long)l, (u_long)r);
    return t;
}

/* safely multiply 2 size_t */
size_t smul(size_t l, size_t r)
{
    size_t  t;

    t = l * r;
    if(l && t / l != r)
        bye("integer overflow %lu * %lu.", (u_long)l, (u_long)r);
    return t;
}

#ifdef WIN32
void windows_init()
{
    WORD werd;
    WSADATA data;

    werd = MAKEWORD( 2, 2 );
    if( (WSAStartup(werd, &data)) !=0 )
        bye("Failed to start WinSock.");
}
#endif

/* Use this to print debug or diagnostic messages to avoid polluting the user
   stream. */
void loguser(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void logdebug(const char *fmt, ...)
{
    va_list ap;

    fprintf(stderr, "DEBUG: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void die(char *err)
{
    perror(err);
    exit(EXIT_FAILURE);
}

/* adds newline for you */
void bye(const char *fmt, ...)
{
    va_list ap;

    fprintf(stderr, "%s: ", NCAT_NAME);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, " QUITTING.\n");

    exit(EXIT_FAILURE);
}

/* zero out some mem, bzero() is deprecated */
void zmem(void *mem, size_t n)
{
    memset(mem, 0, n);
}

/* Append n bytes starting at s to a malloc-allocated buffer. Reallocates the
   buffer and updates the variables to make room if necessary. */
int strbuf_append(char **buf, size_t *size, size_t *offset, const char *s, size_t n)
{
    assert(*offset <= *size);

    if (n >= *size - *offset) {
        *size += n + 1;
        *buf = (char*) safe_realloc(*buf, *size);
    }

    memcpy(*buf + *offset, s, n);
    *offset += n;
    (*buf)[*offset] = '\0';

    return n;
}

/* Append a '\0'-terminated string as with strbuf_append. */
int strbuf_append_str(char **buf, size_t *size, size_t *offset, const char *s)
{
    return strbuf_append(buf, size, offset, s, strlen(s));
}

/* Do a sprintf at the given offset into a malloc-allocated buffer. Reallocates
   the buffer and updates the variables to make room if necessary. */
int strbuf_sprintf(char **buf, size_t *size, size_t *offset, const char *fmt, ...)
{
    va_list va;
    int n;

    assert(*offset <= *size);

    if (*buf == NULL) {
        *size = 1;
        *buf = (char*) safe_malloc(*size);
    }

    for (;;) {
        va_start(va, fmt);
        n = Vsnprintf(*buf + *offset, *size - *offset, fmt, va);
        va_end(va);
        if (n < 0)
            *size = MAX(*size, 1) * 2;
        else if (n >= *size - *offset)
            *size += n + 1;
        else
            break;
        *buf = (char*) safe_realloc(*buf, *size);
    }
    *offset += n;

    return n;
}

/* Make a new allocated null-terminated string from the bytes [start, end). */
char *mkstr(const char *start, const char *end)
{
    char *s;

    assert(end >= start);
    s = (char *) safe_malloc(end - start + 1);
    memcpy(s, start, end - start);
    s[end - start] = '\0';

    return s;
}

/* This is like strtol or atoi, but it allows digits only. No whitespace, sign,
   or radix prefix. */
long parse_long(const char *s, char **tail)
{
    if (!isdigit(*s)) {
        *tail = (char *) s;
        return 0;
    }

    return strtol(s, (char **) tail, 10);
}

/* Return true if the given address is a local one. */
int addr_is_local(const struct sockaddr_storage *ss)
{
    struct addrinfo hints = { 0 }, *addrs, *addr;
    char hostname[128];

    /* Check loopback addresses. */
    if (ss->ss_family == AF_INET) {
        const struct sockaddr_in *s_in = (const struct sockaddr_in *) ss;
        if ((ntohl(s_in->sin_addr.s_addr) & 0xFF000000UL) == 0x7F000000UL)
            return 1;
        if (ntohl(s_in->sin_addr.s_addr) == 0x00000000UL)
            return 1;
    }
#ifdef HAVE_IPV6
    else if (ss->ss_family == AF_INET6) {
        const struct sockaddr_in6 *s_in6 = (const struct sockaddr_in6 *) ss;

        if (memcmp(&s_in6->sin6_addr, &in6addr_any, sizeof(s_in6->sin6_addr)) == 0
            || memcmp(&s_in6->sin6_addr, &in6addr_loopback, sizeof(s_in6->sin6_addr)) == 0)
            return 1;
    }
#endif

    /* Check addresses assigned to the local host name. */
    if (gethostname(hostname, sizeof(hostname)) == -1)
        return 0;
    hints.ai_family = ss->ss_family;
    if (getaddrinfo(hostname, NULL, &hints, &addrs) != 0)
        return 0;
    for (addr = addrs; addr != NULL; addr = addr->ai_next) {
        if (addr->ai_family != ss->ss_family)
            continue;
        if (ss->ss_family == AF_INET) {
            const struct sockaddr_in *s_in = (const struct sockaddr_in *) ss;
            if (s_in->sin_addr.s_addr == ((struct sockaddr_in *) addr->ai_addr)->sin_addr.s_addr)
                break;
        } else if (ss->ss_family == AF_INET6) {
            const struct sockaddr_in6 *s_in6 = (const struct sockaddr_in6 *) ss;
            if (memcmp(&s_in6->sin6_addr, &((struct sockaddr_in6 *) addr->ai_addr)->sin6_addr, sizeof(s_in6->sin6_addr)) == 0)
                break;
        }
    }
    freeaddrinfo(addrs);
    if (addr != NULL)
        return 1;

    return 0;
}

/* Converts an IP address given in a sockaddr_storage to an IPv4 or
   IPv6 IP address string.  Since a static buffer is returned, this is
   not thread-safe and can only be used once in calls like printf()
*/
const char *inet_socktop(const struct sockaddr_storage *ss) {
  static char buf[INET6_ADDRSTRLEN];
  struct sockaddr_in *sin = (struct sockaddr_in *) ss;
#if HAVE_IPV6
  struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) ss;
#endif

  if (inet_ntop(sin->sin_family, (ss->ss_family == AF_INET)?
                (char *) &sin->sin_addr :
#if HAVE_IPV6
                (char *) &sin6->sin6_addr,
#else
                (char *) NULL,
#endif /* HAVE_IPV6 */
                buf, sizeof(buf)) == NULL) {
    bye("Failed to convert address to presentation format!  Error: %s.", strerror(socket_errno()));
  }
  return buf;
}

/* Returns the port number in HOST BYTE ORDER based on the ss's family */
unsigned short inet_port(struct sockaddr_storage *ss)
{
    if (ss->ss_family == AF_INET)
        return ntohs(((struct sockaddr_in *) ss)->sin_port);
#ifdef HAVE_IPV6
    else if (ss->ss_family == AF_INET6)
        return ntohs(((struct sockaddr_in6 *) ss)->sin6_port);
#endif

    bye("Invalid address family passed to inet_port().");
    return 0;
}

int do_listen(int type)
{
    int sock = 0, option_on = 1;

    if(type != SOCK_STREAM && type != SOCK_DGRAM)
        return -1;

    /* We need a socket that can be inherited by child processes in
       ncat_exec_win.c, for --exec and --sh-exec. inheritable_socket is from
       nbase. */
    sock = inheritable_socket(srcaddr.ss_family, type, 0);

    Setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option_on, sizeof(int));

    Bind(sock, (struct sockaddr *) &srcaddr, (int) srcaddrlen);

    if(type == SOCK_STREAM)
        Listen(sock, BACKLOG);

    if(o.verbose)
        loguser("Listening on %s:%hu\n", inet_socktop(&srcaddr), inet_port(&srcaddr));

    return sock;
}

unsigned char *buildsrcrte(struct in_addr dstaddr, struct in_addr routes[],
                  int numroutes, int ptr, size_t *len)
{
    int x;
    unsigned char *opts, *p;

    *len = (numroutes + 1) * sizeof(struct in_addr) + 4;

    if (numroutes > 8)
        bye("Bad number of routes passed to buildsrcrte().");

    opts = (unsigned char *) safe_malloc(*len);
    p = opts;

    zmem(opts, *len);

    *p++ = 0x01; /* IPOPT_NOP, for alignment */
    *p++ = 0x83; /* IPOPT_LSRR */
    *p++ = (char) (*len - 1); /* subtract nop */
    *p++ = (char) ptr;

    for (x = 0; x < numroutes; x++) {
        memcpy(p, &routes[x], sizeof(routes[x]));
        p += sizeof(routes[x]);
    }

    memcpy(p, &dstaddr, sizeof(dstaddr));

    return opts;
}

int allow_access(const struct sockaddr_storage *ss)
{
    /* A host not in the allow set is denied, but only if the --allow or
       --allowfile option was given. */
    if (o.allow && !addrset_contains(&o.allowset, (const struct sockaddr *) ss))
        return 0;
    if (addrset_contains(&o.denyset, (const struct sockaddr *) ss))
        return 0;

    return 1;
}

/*
 * ugly code to maintain our list of fds so we can have proper fdmax for
 * select().  really this should be generic list code, not this silly bit of
 * stupidity. -sean
 */

/* add a descriptor to our list */
int add_fd(fd_list_t *fdl, int fd, void *ssl)
{
    if(fdl->nfds >= fdl->maxfds)
        return -1;

    fdl->fds[fdl->nfds].fd = fd;
#ifdef HAVE_OPENSSL
    fdl->fds[fdl->nfds].ssl = (SSL *) ssl;
#endif

    fdl->nfds++;

    if(fd > fdl->fdmax)
        fdl->fdmax = fd;

    if(o.debug > 1)
        logdebug("Added fd %d to list, nfds %d, maxfd %d\n", fd, fdl->nfds, fdl->fdmax);
    return 0;
}

/* remove a descriptor from our list */
int rm_fd(fd_list_t *fdl, int fd)
{
    int x = 0, last = fdl->nfds;

    /* make sure we have a list */
    if(last == 0)
        bye("Program bug: Trying to remove fd from list with no fds.");

    /* find the fd in the list */
    for(x = 0; x < last; x++)
        if(fdl->fds[x].fd == fd)
            break;

    /* make sure we found it */
    if(x == last)
        bye("Program bug: fd (%d) not on list.", fd);

    /* remove it, does nothing if(last == 1) */
    if(o.debug > 1)
        logdebug("Swapping fd[%d] (%d) with fd[%d] (%d)\n",
                 x, fdl->fds[x].fd, last - 1, fdl->fds[last - 1].fd);
    fdl->fds[x] = fdl->fds[last - 1];

    fdl->nfds--;

    /* was it the max */
    if(fd == fdl->fdmax)
        fdl->fdmax = get_maxfd(fdl);

    if(o.debug > 1)
        logdebug("Removed fd %d from list, nfds %d, maxfd %d\n", fd, fdl->nfds, fdl->fdmax);
    return 0;
}

/* find the max descriptor in our list */
int get_maxfd(fd_list_t *fdl)
{
    int x = 0,  max = -1,   nfds = fdl->nfds;

    for(x = 0; x < nfds; x++)
        if(fdl->fds[x].fd > max)
            max = fdl->fds[x].fd;

    return max;
}

struct fdinfo *get_fdinfo(const fd_list_t *fdl, int fd)
{
    int x;

    for (x = 0; x < fdl->nfds; x++)
        if (fdl->fds[x].fd == fd)
            return &fdl->fds[x];

    return NULL;
}

void init_fdlist(fd_list_t *fdl, int maxfds)
{
    fdl->fds = (struct fdinfo *) Calloc(maxfds, sizeof(struct fdinfo));
    fdl->nfds = 0;
    fdl->fdmax = -1;
    fdl->maxfds = maxfds;

    if(o.debug > 1)
        logdebug("Initialized fdlist with %d maxfds\n", maxfds);
}

void free_fdlist(fd_list_t *fdl)
{
    free(fdl->fds);
    fdl->nfds = 0;
    fdl->fdmax = -1;
}


/*  If any changes need to be made to EOL sequences to comply with --crlf
 *  then dst will be populated with the modified src, len will be adjusted
 *  accordingly and the return will be non-zero.
 *  Returns 0 if changes were not made - len and dst will remain untouched.
 */
int fix_line_endings(char *src, int *len, char **dst)
{
    char *tmp = NULL;
    int fix_count;
    int i,j;
    int num_bytes = *len;

    /* get count of \n without matching \r */
    fix_count = 0;
    for (i = 0; i < num_bytes; i++) {
        if (src[i] == '\n' && (i == 0 || src[i-1] != '\r'))
            fix_count++;
    }
    if (fix_count <= 0 ) return 0;

    /* now insert matching \r */
    *dst = (char *) safe_malloc(num_bytes + fix_count);
    j=0;

    for (i = 0; i < num_bytes; i++) {
        if (src[i] == '\n' && (i == 0 || src[i-1] != '\r')) {
            memcpy(*dst+j, "\r\n", 2);
            j += 2;
        } else {
            memcpy(*dst+j, src+i, 1);
            j++;
        }
    }
    *len += fix_count;

    return 1;
}
