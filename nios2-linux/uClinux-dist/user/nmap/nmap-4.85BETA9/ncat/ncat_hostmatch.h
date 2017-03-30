/* $Id: ncat_hostmatch.h 12253 2009-02-22 03:19:20Z david $ */

#ifndef _NCAT_HOSTMATCH_H
#define _NCAT_HOSTMATCH_H

#include <limits.h>
#include <stdio.h>
#ifndef WIN32
#include <sys/socket.h>
#endif

struct addrset_elem;

/* A set of addresses. Used to match against allow/deny lists. */
struct addrset {
    /* Linked list of struct addset_elem. */
    struct addrset_elem *head;
};

/* We use bit vectors to represent what values are allowed in an IPv4 octet.
   Each vector is built up of an array of bitvector_t (any convenient integer
   type). */
typedef unsigned long bitvector_t;
/* A 256-element bit vector, representing legal values for one octet. */
typedef bitvector_t octet_bitvector[(256 - 1) / (sizeof(unsigned long) * CHAR_BIT) + 1];

enum addrset_elem_type {
    ADDRSET_TYPE_IPV4_BITVECTOR,
#ifdef HAVE_IPV6
    ADDRSET_TYPE_IPV6_NETMASK,
#endif
};

/* A chain of tests for set inclusion. If one test is passed, the address is in
   the set. */
struct addrset_elem {
    enum addrset_elem_type type;
    union {
        struct {
            /* A bit vector for each address octet. */
            octet_bitvector bits[4];
        } ipv4;
#ifdef HAVE_IPV6
        struct {
            struct in6_addr addr;
            struct in6_addr mask;
        } ipv6;
#endif
    } u;
    struct addrset_elem *next;
};

extern void addrset_init(struct addrset *set);

extern void addrset_free(struct addrset *set);

extern int addrset_add_spec(struct addrset *set, const char *spec);

extern int addrset_add_file(struct addrset *set, FILE *fd);

extern int addrset_contains(const struct addrset *set, const struct sockaddr *sa);

#endif
