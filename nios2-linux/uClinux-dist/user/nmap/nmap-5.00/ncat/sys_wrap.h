/* wrappers for system and libC functions calls */

/* $Id: sys_wrap.h 13551 2009-06-05 22:40:21Z david $ */

#ifndef SYS_WRAP_H
#define SYS_WRAP_H

#include "nbase.h"

#include "util.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#else
#include <gnuc.h>
#define pid_t int
#define mode_t int
#define uid_t int
#define socklen_t int
#define uint16_t int
#define ssize_t int
#include <WinDef.h>
#endif

#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* need an autoconf to check for this */
typedef void (*sighandler_t)(int);

int Bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
void * Calloc(size_t nmemb, size_t size);
int Close(int fd);
int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
int Dup2(int oldfd, int newfd);
int Listen(int s, int backlog);
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Recv(int s, void *buf, size_t len, int flags);
ssize_t Recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
ssize_t Send(int s, const void *buf, size_t len, int flags);
int Setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
sighandler_t Signal(int signum, sighandler_t handler);
int Socket(int domain, int type, int protocol);
char * Strdup(const char *s);
long Strtol(const char *nptr, char **endptr, int base);
pid_t Waitpid(pid_t pid, int *status, int options);
ssize_t Write(int fd, const void *buf, size_t count);

#endif
