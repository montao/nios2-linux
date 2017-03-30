/* This file contains Windows-specific implementations of some functions. */

#include "nbase.h"
#include "ncat.h"

#ifdef HAVE_OPENSSL

int ssl_load_default_ca_certs(SSL_CTX *ctx)
{
    char buf[1024];
    char *bundlename;
    int n, rc;
    size_t size, offset;

    /* Get the executable's filename. */
    n = GetModuleFileName(GetModuleHandle(0), buf, sizeof(buf));
    if (n == 0 || n == sizeof(buf))
        return -1;

    bundlename = path_get_dirname(buf);
    bundlename = (char *) safe_realloc(bundlename, 1024);
    offset = strlen(bundlename);
    size = offset + 1;
    strbuf_sprintf(&bundlename, &size, &offset, "\\%s", NCAT_CA_CERTS_FILE);

    if (o.debug)
        logdebug("Using trusted CA certificates from %s.\n", bundlename);
    rc = SSL_CTX_load_verify_locations(ctx, bundlename, NULL);
    if (rc != 1) {
        if (o.debug)
            logdebug("Unable to load trusted CA certificates from %s: %s\n",
                bundlename, ERR_error_string(ERR_get_error(), NULL));
    }
    free(bundlename);

    return rc == 1 ? 0 : -1;
}
#endif
