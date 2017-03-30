/* This file contains Windows-specific implementations of some functions. */

#include "nbase.h"
#include "ncat.h"

void set_lf_mode(void)
{
    /* _O_TEXT (the default setting) converts \r\n to \n on input, making the
       terminal look like a Unix terminal. However, use _O_BINARY if stdin is
       not a terminal, to avoid breaking data from a pipe or other source. */
    if (isatty(STDIN_FILENO))
        _setmode(STDIN_FILENO, _O_TEXT);
    else
        _setmode(STDIN_FILENO, _O_BINARY);
    /* Do not translate \n to \r\n on output. */
    _setmode(STDOUT_FILENO, _O_BINARY);
}

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
