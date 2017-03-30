/* $Id: sys_wrap.c 13551 2009-06-05 22:40:21Z david $ */

#include <limits.h>

#include "sys_wrap.h"
#include "util.h"

int Bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
    int ret;

    ret = bind(sockfd, my_addr, addrlen);
    if(ret < 0)
        die("bind");

    return ret;
}

void * Calloc(size_t nmemb, size_t size)
{
    void    *ret;

    /* older libcs don't check for int overflow */
    smul(nmemb, size);

    ret = calloc(nmemb, size);
    if(ret == NULL)
        die("calloc");

    return ret;
}

int Close(int fd)
{
    if(close(fd) < 0)
        die("close");

    return 0;
}

int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
{
    if(connect(sockfd, serv_addr, addrlen) < 0)
        die("connect");

    return 0;
}

int Dup2(int oldfd, int newfd)
{
    int ret;

    ret = dup2(oldfd, newfd);
    if(ret < 0)
        die("dup2");

    return ret;
}

int Listen(int s, int backlog)
{
    if(listen(s, backlog) < 0)
        die("listen");

    return 0;
}

int Open(const char *pathname, int flags, mode_t mode)
{
    int ret;

    ret = open(pathname, flags, mode);
    if(ret < 0)
        die("open");

    return ret;
}

ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t ret;

    ret = read(fd, buf, count);
    if(ret < 0)
        die("read");

    return ret;
}

ssize_t Recv(int s, void *buf, size_t len, int flags)
{
    ssize_t ret;

    ret = recv(s, (char*)buf, len, flags);
    if(ret < 0)
        die("recv");

    return ret;
}

ssize_t Recvfrom(int s, void *buf, size_t len, int flags,
                    struct sockaddr *from, socklen_t *fromlen)
{
    ssize_t ret;

    ret = recvfrom(s, (char*)buf, len, flags, from, fromlen);
    if(ret < 0)
        die("recvfrom");

    return ret;
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                    struct timeval *timeout)
{
    int ret;

   do{
        ret = select(n, readfds, writefds, exceptfds, timeout);
        if(ret < 0 && errno != EINTR)
            die("select");
    }while(ret < 0 && errno == EINTR);

    return ret;
}

ssize_t Send(int s, const void *buf, size_t len, int flags)
{
    ssize_t ret;

    ret = send(s, (const char*)buf, len, flags);
    if(ret < 0)
        die("send");

    return ret;
}

int Setsockopt(int s, int level, int optname, const void *optval,
                    socklen_t optlen)
{
    int ret;

    ret = setsockopt(s, level, optname, (const char*)optval, optlen);
    if(ret < 0)
        die("setsockopt");

    return ret;
}

sighandler_t Signal(int signum, sighandler_t handler)
{
    sighandler_t    ret;

    ret = signal(signum, handler);
    if(ret == SIG_ERR)
        die("signal");

    return ret;
}


int Socket(int domain, int type, int protocol)
{
    int ret;

    ret = socket(domain, type, protocol);
    if(ret < 0)
        die("socket");

    return ret;
}

char * Strdup(const char *s)
{
    char    *ret;

    ret = strdup(s);
    if(ret == NULL)
        die("strdup");

    return ret;
}

long Strtol(const char *nptr, char **endptr, int base)
{
    long    ret;

    ret = strtol(nptr, endptr, base);
    if(ret == LONG_MIN || ret == LONG_MAX)
        die("strtol");

    return ret;
}

pid_t Waitpid(pid_t pid, int *status, int options)
{
#ifndef WIN32
    return waitpid(pid, status, options);
#else
    return _cwait (status, pid, _WAIT_CHILD);
#endif
}

ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t ret = write(fd, buf, count);

    if(ret < 0)         /* we don't bail if < count bytes written */
        die("write");

    return ret;
}
