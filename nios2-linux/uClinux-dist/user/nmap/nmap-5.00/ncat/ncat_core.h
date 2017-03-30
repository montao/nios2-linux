/* $Id: ncat_core.h 13651 2009-06-10 03:03:01Z david $ */

#include "nsock.h"
#include "nbase.h"
#include "ncat_hostmatch.h"
#include "util.h"

extern struct sockaddr_storage srcaddr;
extern size_t srcaddrlen;
extern struct sockaddr_storage targetss;
extern size_t targetsslen;

extern struct sockaddr_storage httpconnect;
extern struct sockaddr_storage socksconnect;

struct options {
    unsigned short portno;

    int verbose;
    int debug;
    char *target;
    int af;
    int broker;
    int listen;
    int keepopen;
    int sendonly;
    int recvonly;
    int telnet;
    int udp;
    int linedelay;
    int chat;
    int nodns;
    int normlogfd;
    int hexlogfd;
    int idletimeout;
    int crlf;
    /* Were any hosts specifically allowed? If so, deny all others. */
    int allow;
    int deny;
    struct addrset allowset;
    struct addrset denyset;
    int httpserver;

    /* Loose source-routing stuff */
    struct in_addr srcrtes[8];
    int numsrcrtes;
    int srcrteptr;

    /* Maximum number of simultaneous connections */
    int conn_limit;
    int conntimeout;

    char *cmdexec;
    int shellexec;
    char *proxy_auth;
    char *proxytype;

    int ssl;
    char *sslcert;
    char *sslkey;
    int sslverify;
    char *ssltrustfile;
};

extern struct options o;

/* Initializes global options to their default values. */
void options_init(void);

/* Tries to resolve the given name (or literal IP) into a sockaddr structure.
   Pass 0 for the port if you don't care. Returns 0 if hostname cannot be
   resolved. */
int resolve(char *hostname, unsigned short port,
            struct sockaddr_storage *ss, size_t *sslen, int af);

/* Broadcast a message to all the descriptors in fds. Returns -1 if any of the
   sends failed. */
extern int broadcast(const fd_set *fds, const fd_list_t *fdlist, const char *msg, size_t size);

/* Do telnet WILL/WONT DO/DONT negotiations */
extern void dotelnet(int s, unsigned char *buf, size_t bufsiz);

/* Return 1 if user is root, otherwise 0. */
extern int ncat_checkuid();

/* sleep(), usleep(), msleep(), Sleep() -- all together now, "portability".
 *
 * There is no upper or lower limit to the delayval, so if you pass in a short
 * length of time <100ms, then you're likely going to get odd results.
 * This is because the Linux timeslice is 10ms-200ms. So don't expect
 * it to return for at least that long.
 *
 * Block until the specified time has elapsed, then return 1.
 */
extern int ncat_delay_timer(int delayval);

/* Open a logfile for writing.
 * Return the open file descriptor. */
extern int ncat_openlog(char *logfile);

extern void ncat_log_send(const char *data, size_t len);

extern void ncat_log_recv(const char *data, size_t len);

extern int ncat_hostaccess(char *matchaddr, char *filename, char *remoteip);

/* Make it so that line endings read from a console are always \n (not \r\n).
   Defined in ncat_posix.c and ncat_win.c. */
extern void set_lf_mode(void);
