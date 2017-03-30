/* utility functions */

/* $Id: util.h 13631 2009-06-09 01:08:45Z jah $ */

#ifndef UTIL_H_
#define UTIL_H_

/* This is a temporary hack, as we'll probably want to do
   something different for Win32 config */
#ifdef WIN32
#define HAVE_OPENSSL 1
#else
#include "config.h"
#endif

#include "nbase.h"
#ifndef WIN32
#include <sys/types.h>
#include <netinet/in.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

/* add/multiply unsigned values safely */
size_t sadd(size_t, size_t);
size_t smul(size_t, size_t);

#ifdef WIN32
void windows_init();
#endif

void loguser(const char *fmt, ...);
void logdebug(const char *fmt, ...);

/* handle errors */
void die(char *);

void bye(const char *, ...);

/* zero out some memory, bzero() is deprecated */
void zmem(void *, size_t);

int strbuf_append(char **buf, size_t *size, size_t *offset, const char *s, size_t n);

int strbuf_append_str(char **buf, size_t *size, size_t *offset, const char *s);

int strbuf_sprintf(char **buf, size_t *size, size_t *offset, const char *fmt, ...);

char *mkstr(const char *start, const char *end);

long parse_long(const char *s, char **tail);

int addr_is_local(const struct sockaddr_storage *ss);

const char *inet_socktop(const struct sockaddr_storage *);

unsigned short inet_port(struct sockaddr_storage *);

int do_listen(int);

unsigned char *buildsrcrte(struct in_addr dstaddr, struct in_addr routes[],
                  int numroutes, int ptr, size_t *len);

int allow_access(const struct sockaddr_storage *ss);

struct fdinfo {
    int fd;
#ifdef HAVE_OPENSSL
    SSL *ssl;
#endif
};

typedef struct fd_list {
    struct fdinfo *fds;
    int nfds, maxfds, fdmax;
} fd_list_t;

int add_fd(fd_list_t *, int, void*);
int rm_fd(fd_list_t *, int);
void free_fdlist(fd_list_t *);
void init_fdlist(fd_list_t *, int);
int get_maxfd(fd_list_t *);
struct fdinfo *get_fdinfo(const fd_list_t *, int);

#endif

int fix_line_endings(char *src, int *len, char **dst);
