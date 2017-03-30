/* $Id: ncat_ssl.h 13714 2009-06-12 22:21:39Z david $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

#define NCAT_CA_CERTS_FILE "ca-bundle.crt"

enum {
    SHA1_BYTES = 160 / 8,
    /* 40 bytes for hex digits and 9 bytes for ' '. */
    SHA1_STRING_LENGTH = SHA1_BYTES * 2 + (SHA1_BYTES / 2 - 1)
};

extern SSL_CTX *setup_ssl_listen(void);

extern SSL *new_ssl(int fd);

extern int ssl_post_connect_check(SSL *ssl, const char *hostname);

extern char *ssl_cert_fp_str_sha1(const X509 *cert, char *strbuf, size_t len);

extern int ssl_load_default_ca_certs(SSL_CTX *ctx);

#endif
