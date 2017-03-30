/* This file includes POSIX-specific definitions of certain functions. */

#include <assert.h>

#include "ncat.h"

static char **cmdline_split(const char *cmdexec);

/* fork and exec a child process with netexec. Close the given file descriptor
   in the parent process. Return the child's PID or -1 on error. */
int netrun(int fd, char *cmdexec)
{
    int pid;

    errno = 0;
    pid = vfork();
    if (pid == 0) {
        /* In the child process. */
        netexec(fd, cmdexec);
    }

    if (pid == -1 && o.verbose)
        logdebug("Error in fork: %s\n", strerror(errno));

    Close(fd);

    return pid;
}

/* exec the given command line. Before the exec, redirect stdin, stdout, and
   stderr to the given file descriptor. Never returns. */
void netexec(int fd, char *cmdexec)
{
    /* If we're executing through /bin/sh */
    if (o.shellexec) {
        if (o.debug)
            logdebug("Executing with shell: %s\n", cmdexec);

        /* rearrange stdin/stdout/stderr */
        Dup2(fd, STDIN_FILENO);
        Dup2(fd, STDOUT_FILENO);
        Dup2(fd, STDERR_FILENO);

        execl("/bin/sh", "sh", "-c", cmdexec, NULL);
    } else {
        char **cmdargs;

        if (o.debug)
            logdebug("Executing: %s\n", cmdexec);

        cmdargs = cmdline_split(cmdexec);

        /* rearrange stdin/stdout/stderr */
        Dup2(fd, STDIN_FILENO);
        Dup2(fd, STDOUT_FILENO);
        Dup2(fd, STDERR_FILENO);

        execv(cmdargs[0], cmdargs);
    }
    /* exec failed.*/
    die("exec");
}

/* Split a command line into an array suitable for handing to execv. */
static char **cmdline_split(const char *cmdexec)
{
    char *token,    *newtoken,  **cmdargs,  *cmdbin;
    char *cmdexec_local,    *cmdexec_path;
    int x = 1,  arg_count = 0,  y = 0,  path_count = 0, maxlen = 0;

    /* FIXME fix this command parsing code it is not pretty */
    maxlen = strlen(cmdexec);
    cmdexec_local = Strdup(cmdexec);
    cmdexec_path = Strdup(cmdexec);

    /* parse command line into cmd + args */
    if ((token = strtok(cmdexec_path, " ")) != NULL) {
        do {
            /* position of the end of token */
            y = (strlen(token)) - 1;

            /* if token ends with an escape */
            if (token[y] == '\\') {
                path_count++;
            } else {
                arg_count++;
            }
        } while ((token = strtok(NULL, " ")) != NULL);
    }

    /* malloc space based on supplied command/arguments */
    cmdbin = (char *)safe_malloc((path_count + 2) * sizeof(cmdbin));
    cmdargs = (char **)safe_malloc((arg_count + 2) * sizeof(cmdargs));
    newtoken = (char *)Calloc(((maxlen + arg_count + path_count) * 4), sizeof(char));

    /* assemble arg vector */
    if ((token = strtok(cmdexec_local, " ")) != NULL) {
        int ar = 0;

        do {
            /* craft the path & executable name, handling whitespaces. */
            if (ar <= path_count) {
                y = (strlen(token)) - 1;

                if (token[y] == '\\') {
                    strncat(newtoken, token, (strlen(token) - 1));
                    strcat(newtoken, " ");
                } else
                    strncat(newtoken, token, (strlen(token)));

                if (o.debug > 1)
                    logdebug("Executable path: %s\n", newtoken);
                ar++;
            } else {
                /* craft the arguments to the command */

                if (o.debug > 1)
                    logdebug("Command argument: %s\n", token);

                cmdargs[x++] = (char *) token;
            }
        } while ((token = strtok(NULL, " ")) != NULL);
        cmdargs[0] = newtoken;
    }

    cmdargs[x] = NULL;

    return cmdargs;
}

void set_lf_mode(void)
{
    /* Nothing needed. */
}

#ifdef HAVE_OPENSSL

#define NCAT_CA_CERTS_PATH (NCAT_DATADIR "/" NCAT_CA_CERTS_FILE)

int ssl_load_default_ca_certs(SSL_CTX *ctx)
{
    int rc;

    if (o.debug)
        logdebug("Using system default trusted CA certificates and those in %s.\n", NCAT_CA_CERTS_PATH);

    /* Load distribution-provided defaults, if any. */
    assert(SSL_CTX_set_default_verify_paths(ctx) > 0);

    /* Also load the trusted certificates we ship. */
    rc = SSL_CTX_load_verify_locations(ctx, NCAT_CA_CERTS_PATH, NULL);
    if (rc != 1) {
        if (o.debug)
            logdebug("Unable to load trusted CA certificates from %s: %s\n",
                NCAT_CA_CERTS_PATH, ERR_error_string(ERR_get_error(), NULL));
        return -1;
    }

    return 0;
}
#endif
