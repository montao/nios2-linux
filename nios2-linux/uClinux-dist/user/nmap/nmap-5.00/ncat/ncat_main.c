/* $Id: ncat_main.c 13816 2009-06-18 14:45:13Z david $ */

#include "nsock.h"
#include "ncat.h"
#include "util.h"
#include "sys_wrap.h"

#include <getopt.h>

#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#ifndef WIN32
#include <netdb.h>
#endif
#include <fcntl.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

static int ncat_connect_mode(void);
static int ncat_listen_mode(void);

/* Determines if it's parsing HTTP or SOCKS by looking at defport */
static void parseproxy(char *str, struct sockaddr_storage *ss, unsigned short defport)
{
    char *c = strrchr(str, ':'), *ptr;
    int httpproxy = (defport == DEFAULT_PROXY_PORT);
    unsigned short portno;
    size_t sslen;

    ptr = str;

    if (c)
        *c = 0;

    if (c && strlen((c + 1)))
        portno = (unsigned short) atoi(c + 1);
    else
        portno = defport;

    if (!resolve(ptr, portno, ss, &sslen, o.af)) {
        loguser("%s: Could not resolve proxy \"%s\".\n", NCAT_NAME, ptr);
        if (o.af == AF_INET6 && httpproxy)
            loguser("Did you specify the port number? It's required for IPv6.\n");
        exit(EXIT_FAILURE);
    }
}

/* These functions implement a simple linked list to hold allow/deny
   specifications until the end of option parsing. */
struct host_list_node {
    /* If false, then spec is the name of a file containing host patterns. */
    int is_filename;
    char *spec;
    struct host_list_node *next;
};

static void host_list_add_spec(struct host_list_node **list, char *spec)
{
    struct host_list_node *node = (struct host_list_node *) safe_malloc(sizeof(*node));
    node->is_filename = 0;
    node->spec = spec;
    node->next = *list;
    *list = node;
}

static void host_list_add_filename(struct host_list_node **list, char *filename)
{
    struct host_list_node *node = (struct host_list_node *) safe_malloc(sizeof(*node));
    node->is_filename = 1;
    node->spec = filename;
    node->next = *list;
    *list = node;
}

static void host_list_free(struct host_list_node *list)
{
    struct host_list_node *next;
    for ( ; list != NULL; list = next) {
        next = list->next;
        free(list);
    }
}

static void host_list_to_set(struct addrset *set, struct host_list_node *list)
{
    struct host_list_node *node;

    for (node = list; node != NULL; node = node->next) {
        if (node->is_filename) {
            FILE *fd;

            fd = fopen(node->spec, "r");
            if (fd == NULL)
                bye("can't open %s: %s.", node->spec, strerror(errno));
            if (!addrset_add_file(set, fd))
                bye("error in hosts file %s.", node->spec);
            fclose(fd);
        } else {
            char *spec, *commalist;

            commalist = node->spec;
            while ((spec = strtok(commalist, ",")) != NULL) {
                commalist = NULL;
                if (!addrset_add_spec(set, spec))
                    bye("error in host specification \"%s\".", node->spec);
            }
        }
    }
}

static int print_banner(FILE *fp)
{
    return fprintf(fp, "%s version %s ( %s )\n", NCAT_NAME, NCAT_VERSION, NCAT_URL);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in *sin = (struct sockaddr_in *) &targetss;
#ifdef HAVE_IPV6
    static struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) &targetss;
#endif

    /* We have to buffer the lists of hosts to allow and deny until after option
       parsing is done. Adding hosts to an addrset can require name resolution,
       which may differ as a result of options like -n and -6. */
    struct host_list_node *allow_host_list = NULL;
    struct host_list_node *deny_host_list = NULL;

    int srcport = -1;
    char *source = NULL;
    char *proxyaddr = NULL;

    /* Set default options. */
    options_init();

    struct option long_options[] = {
        {"4",               no_argument,        NULL,         '4'},
        {"6",               no_argument,        NULL,         '6'},
        {"crlf",            no_argument,        NULL,         'C'},
        {"g",               required_argument,  NULL,         'g'},
        {"G",               required_argument,  NULL,         'G'},
        {"exec",            required_argument,  NULL,         'e'},
        {"sh-exec",         required_argument,  NULL,         'c'},
        {"max-conns",       required_argument,  NULL,         'm'},
        {"help",            no_argument,        NULL,         'h'},
        {"delay",           required_argument,  NULL,         'd'},
        {"listen",          no_argument,        NULL,         'l'},
        {"output",          required_argument,  NULL,         'o'},
        {"hex-dump",        required_argument,  NULL,         'x'},
        {"idle-timeout",    required_argument,  NULL,         'i'},
        {"keep-open",       no_argument,        NULL,         'k'},
        {"recv-only",       no_argument,        &o.recvonly,  1},
        {"source-port",     required_argument,  NULL,         'p'},
        {"source",          required_argument,  NULL,         's'},
        {"send-only",       no_argument,        &o.sendonly,  1},
        {"broker",          no_argument,        &o.broker,    1},
        {"chat",            no_argument,        NULL,         0},
        {"talk",            no_argument,        NULL,         0},
        {"deny",            required_argument,  NULL,         0},
        {"denyfile",        required_argument,  NULL,         0},
        {"allow",           required_argument,  NULL,         0},
        {"allowfile",       required_argument,  NULL,         0},
        {"telnet",          no_argument,        NULL,         't'},
        {"udp",             no_argument,        NULL,         'u'},
        {"version",         no_argument,        NULL,         0},
        {"verbose",         no_argument,        NULL,         'v'},
        {"wait",            required_argument,  NULL,         'w'},
        {"nodns",           no_argument,        NULL,         'n'},
        {"proxy",           required_argument,  NULL,         0},
        {"proxy-type",      required_argument,  NULL,         0},
        {"proxy-auth",      required_argument,  NULL,         0},
#ifdef HAVE_OPENSSL
        {"ssl",             no_argument,        &o.ssl,       1},
        {"ssl-cert",        required_argument,  NULL,         0},
        {"ssl-key",         required_argument,  NULL,         0},
        {"ssl-verify",      no_argument,        NULL,         0},
        {"ssl-trustfile",   required_argument,  NULL,         0},
#endif
        {0, 0, 0, 0}
    };

#ifdef WIN32
    windows_init();
#endif

    while (1) {
        /* handle command line arguments */
        int option_index;
        int c = getopt_long(argc, argv, "46Cc:e:g:G:i:km:hp:d:lo:x:ts:uvw:n",
                            long_options, &option_index);

        /* That's the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case '4':
            o.af = AF_INET;
            break;
        case '6':
#ifdef HAVE_IPV6
            o.af = AF_INET6;
#else
            bye("-6 chosen when IPv6 wasn't compiled in.");
#endif
            break;
        case 'C':
            o.crlf = 1;
            break;
        case 'c':
            o.shellexec = 1;
            /* fall through */
        case 'e':
            o.cmdexec = optarg;
            break;
        case 'g': {
            char *a = strtok(optarg, ",");
            do {
                struct sockaddr_in addr;
                size_t sslen;
                if (!resolve(a, 0, (struct sockaddr_storage *) &addr, &sslen, AF_INET))
                    bye("Sorry, could not resolve source route hop %s.", a);
                o.srcrtes[o.numsrcrtes] = addr.sin_addr;
            } while (o.numsrcrtes++ <= 8 && (a = strtok(NULL, ",")));
            if (o.numsrcrtes > 8)
                bye("Sorry, you gave too many source route hops.");
            break;
        }
        case 'G':
            o.srcrteptr = atoi(optarg);
            if (o.srcrteptr < 4 || (o.srcrteptr % 4) || o.srcrteptr > 28)
                bye("Invalid source-route hop pointer %d.", o.srcrteptr);
            break;
        case 'k':
            o.keepopen = 1;
            break;
        case 'm':
            o.conn_limit = atoi(optarg);
            break;
        case 'd':
            o.linedelay = tval2msecs(optarg);
            if (o.linedelay <= 0)
                bye("Invalid -d delay (must be greater than 0).", optarg);
            break;
        case 'o':
            o.normlogfd = ncat_openlog(optarg);
            break;
        case 'x':
            o.hexlogfd = ncat_openlog(optarg);
            break;
        case 'p':
            srcport = atoi(optarg);
            if (srcport < 0 || srcport > 0xffff)
                bye("Invalid source port %d.", srcport);
            break;
        case 'i':
            o.idletimeout = tval2msecs(optarg);
            if (o.idletimeout <= 0)
                bye("Invalid -i timeout (must be greater than 0).");
            break;
        case 's':
            source = optarg;
            break;
        case 'l':
            o.listen = 1;
            break;
        case 'u':
            o.udp = 1;
            break;
        case 'v':
            /* One -v activites verbose, after that it's debugging. */
            if (o.verbose == 0)
                o.verbose++;
            else
                o.debug++;
            break;
        case 'n':
            o.nodns = 1;
            break;
        case 'w':
            o.conntimeout = tval2msecs(optarg);
            if (o.conntimeout <= 0)
                bye("Invalid -w timeout (must be greater than 0).");
            break;
        case 't':
            o.telnet = 1;
            break;
        case 0:
            if (strcmp(long_options[option_index].name, "version") == 0) {
                print_banner(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(long_options[option_index].name, "proxy") == 0)
            {
                if (proxyaddr)
                    bye("You can't specify more than one --proxy.");
                proxyaddr = Strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "proxy-type") == 0)
            {
                if (o.proxytype)
                        bye("You can't specify more than one --proxy-type.");
                o.proxytype = Strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "proxy-auth") == 0)
            {
                if (o.proxy_auth)
                        bye("You can't specify more than one --proxy-auth.");
                o.proxy_auth = Strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "chat") == 0
                     || strcmp(long_options[option_index].name, "talk") == 0)
            {
                /* --talk is an older name for --chat. */
                o.chat = 1;
                /* --chat implies --broker. */
                o.broker = 1;
            }
            else if (strcmp(long_options[option_index].name, "allow") == 0)
            {
                o.allow = 1;
                host_list_add_spec(&allow_host_list, optarg);
            }
            else if (strcmp(long_options[option_index].name, "allowfile") == 0)
            {
                o.allow = 1;
                host_list_add_filename(&allow_host_list, optarg);
            }
            else if (strcmp(long_options[option_index].name, "deny") == 0)
            {
                host_list_add_spec(&deny_host_list, optarg);
            }
            else if (strcmp(long_options[option_index].name, "denyfile") == 0)
            {
                host_list_add_filename(&deny_host_list, optarg);
            }
#ifdef HAVE_OPENSSL
            else if (strcmp(long_options[option_index].name, "ssl-cert") == 0)
            {
                o.sslcert = Strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "ssl-key") == 0)
            {
                o.sslkey = Strdup(optarg);
            }
            else if (strcmp(long_options[option_index].name, "ssl-verify") == 0)
            {
                o.sslverify = 1;
                o.ssl = 1;
            }
            else if (strcmp(long_options[option_index].name, "ssl-trustfile") == 0)
            {
                if (o.ssltrustfile != NULL)
                    bye("The --ssl-trustfile option may be given only once.");
                o.ssltrustfile = Strdup(optarg);
                /* If they list a trustfile assume they want certificate
                   verification. */
                o.sslverify = 1;
            }
#endif
            break;
        case 'h':
            printf("%s %s ( %s )\n", NCAT_NAME, NCAT_VERSION, NCAT_URL);
            printf(
"Usage: ncat [options] [hostname] [port]\n"
"\n"
"Options taking a time assume milliseconds, unless you append an 's'\n"
"(seconds), 'm' (minutes), or 'h' (hours) to the value (e.g. 30s)\n"
"  -4                         Use IPv4 only\n"
"  -6                         Use IPv6 only\n"
"  -C, --crlf                 Use CRLF for EOL sequence\n"
"  -c, --sh-exec <command>    Executes specified command via /bin/sh\n"
"  -e, --exec <command>       Executes specified command\n"
"  -g hop1[,hop2,...]         Loose source routing hop points (8 max)\n"
"  -G n                       Loose source routing hop pointer (4, 8, 12, ...)\n"
"  -m, --max-conns n          Maximum n simultaneous connections\n"
"  -h, --help                 Display this help screen\n"
"  -d, --delay <time>         Wait between read/writes\n"
"  -o, --output               Dump session data to a file\n"
"  -x, --hex-dump             Dump session data as hex to a file\n"
"  -i, --idle-timeout <time>  Idle read/write timeout\n"
"  -p, --source-port port     Specify source port to use (doesn't affect -l)\n"
"  -s, --source addr          Specify source address to use (doesn't affect -l)\n"
"  -l, --listen               Bind and listen for incoming connections\n"
"  -k, --keep-open            Accept multiple connections in listen mode\n"
"  -n, --nodns                Do not resolve hostnames via DNS\n"
"  -t, --telnet               Answer Telnet negotiations\n"
"  -u, --udp                  Use UDP instead of default TCP\n"
"  -v, --verbose              Set verbosity level (can be used up to 3 times)\n"
"  -w, --wait <time>          Connect timeout\n"
"      --send-only            Only send data, ignoring received; quit on EOF\n"
"      --recv-only            Only receive data, never send anything\n"
"      --allow                Allow specific hosts to connect to Ncat\n"
"      --allowfile            A file of hosts allowed to connect to Ncat\n"
"      --deny                 Hosts to be denied from connecting to Ncat\n"
"      --denyfile             A file of hosts denied from connecting to Ncat\n"
"      --broker               Enable Ncat's Connection Brokering mode\n"
"      --chat                 Start a simple Ncat chat server\n"
"      --proxy <addr[:port]>  Specify address of host to proxy through\n"
"      --proxy-type <type>    Specify proxy type (\"http\" or \"socks4\")\n"
"      --proxy-auth <auth>    Authenticate with HTTP or SOCKS proxy server\n"
#ifdef HAVE_OPENSSL
"      --ssl                  Connect or listen with SSL\n"
"      --ssl-cert             Specify SSL certificate file (PEM) for listening\n"
"      --ssl-key              Specify SSL private key (PEM) for listening\n"
"      --ssl-verify           Verify trust and domain name of certificates\n"
"      --ssl-trustfile        PEM file containing trusted SSL certificates\n"
#endif
"      --version              Display Ncat's version information and exit\n"
"\n"
"See the ncat(1) manpage for full options, descriptions and usage examples\n"
            );
            exit(EXIT_SUCCESS);
        case '?':
            /* Consider unrecognised parameters/arguments as fatal. */
            bye("Try `--help' or man(1) ncat for more information, usage options and help.");
        default:
            /* We consider an unrecognised option fatal. */
            bye("Unrecognised option.");
        }
    }

    if (o.verbose)
        print_banner(stderr);

    /* Will be AF_INET or AF_INET6 when valid */
    memset(&targetss, 0, sizeof(targetss));
    targetss.ss_family = AF_UNSPEC;
    httpconnect = socksconnect = srcaddr = targetss;

    if (proxyaddr) {
      if (!o.proxytype)
          o.proxytype = Strdup("http");

      if (!strcmp(o.proxytype, "http")) {
          /* Parse HTTP proxy address and temporarily store it in httpconnect.  If
           * the proxy server is given as an IPv6 address (not hostname), the port
           * number MUST be specified as well or parsing will break (due to the
           * colons in the IPv6 address and host:port separator).
           */

          parseproxy(proxyaddr, &httpconnect, DEFAULT_PROXY_PORT);
      } else if (!strcmp(o.proxytype, "socks4") || !strcmp(o.proxytype, "4")) {
          /* Parse SOCKS proxy address and temporarily store it in socksconnect */

          parseproxy(proxyaddr, &socksconnect, DEFAULT_SOCKS4_PORT);
      } else {
          bye("Invalid proxy type \"%s\".", o.proxytype);
      }

      free(o.proxytype);
      free(proxyaddr);
    } else {
      if (o.proxytype) {
        if (!o.listen)
          bye("Proxy type (--proxy-type) specified without proxy address (--proxy).");
        if (strcmp(o.proxytype, "http"))
          bye("Invalid proxy type \"%s\".", o.proxytype);
      }
    }

    /* Resolve the given source address */
    if (source) {
        if (o.listen)
            bye("-l and -s are incompatible.  Specify the address and port to bind to like you would a host to connect to.");

        if (!resolve(source, 0, &srcaddr, &srcaddrlen, o.af))
            bye("Could not resolve source address %s.", source);
    }

    if (srcport != -1) {
        if (o.listen)
            bye("-l and -p are incompatible.  Specify the address and port to bind to like you would a host to connect to.");

        if (srcaddr.ss_family == AF_UNSPEC)
            srcaddr.ss_family = o.af;
        if (o.af == AF_INET) {
            ((struct sockaddr_in *) &srcaddr)->sin_port = htons((unsigned short) srcport);
            if (!srcaddrlen)
                srcaddrlen = sizeof(struct sockaddr_in);
        }
#ifdef HAVE_IPV6
        else {
            ((struct sockaddr_in6 *) &srcaddr)->sin6_port = htons((unsigned short) srcport);
            if (!srcaddrlen)
                srcaddrlen = sizeof(struct sockaddr_in6);
        }
#endif
    }

    host_list_to_set(&o.allowset, allow_host_list);
    host_list_free(allow_host_list);
    host_list_to_set(&o.denyset, deny_host_list);
    host_list_free(deny_host_list);

    if (optind == argc) {
        /* Listen defaults to any address and DEFAULT_NCAT_PORT */
        if (!o.listen)
            bye("You must specify a host to connect to.");
    } else {
        /* Resolve hostname if we're given one */
        if (strspn(argv[optind], "0123456789") != strlen(argv[optind])) {
            o.target = argv[optind];
            /* resolve hostname */
            if (!resolve(o.target, 0, &targetss, &targetsslen, o.af))
                bye("Could not resolve hostname %s.", o.target);
            optind++;
        } else {
            if (!o.listen)
                bye("You must specify a host to connect to.");
        }
    }

    if (optind < argc) {
        long long_port = Strtol(argv[optind], NULL, 10);

        if (long_port <= 0 || long_port > 65535)
            bye("Invalid port number.");

        o.portno = (unsigned short) long_port;
    } else {
        /* Default port */
        o.portno = DEFAULT_NCAT_PORT;
    }

    if (o.af == AF_INET)
        sin->sin_port = htons(o.portno);
#ifdef HAVE_IPV6
    else
        sin6->sin6_port = htons(o.portno);
#endif

    /* Since the host we're actually *connecting* to is the proxy server, we
     * need to reverse these address structures to avoid any further confusion
     */
    if (httpconnect.ss_family != AF_UNSPEC) {
        struct sockaddr_storage tmp = targetss;
        targetss = httpconnect;
        httpconnect = tmp;
    } else if (socksconnect.ss_family != AF_UNSPEC) {
        struct sockaddr_storage tmp = targetss;
        targetss = socksconnect;
        socksconnect = tmp;
    }

    if (o.udp) {
        /* Don't allow a false sense of security if someone tries SSL over UDP. */
        if (o.ssl)
            bye("UDP mode does not support SSL.");
        if (o.broker)
            bye("UDP mode does not support connection brokering.\n\
If this feature is important to you, write nmap-dev@insecure.org with a\n\
description of how you intend to use it, as an aid to deciding how UDP\n\
connection brokering should work.");
    }

    /*
     * Ncat obviously just redirects stdin/stdout/stderr, so have no clue what is going on with
     * the underlying application. If it foobars, it's your problem.
     */
    if (o.cmdexec && o.normlogfd != -1)
        bye("Invalid option combination: `-e' and `-o'.");

    /* ditto. */
    if (o.cmdexec && o.hexlogfd != -1)
        bye("Invalid option combination: `-e' and `-x'.");

    /* This doesn't work because we just fork and redirect the child's standard
       input and output to a socket. To support --exec with --ssl we would need
       to fork Ncat code to speak SSL to the socket and plaintext to the
       subprocess. */
    if (o.cmdexec && o.ssl)
        bye("Invalid option combination: `-e' and `--ssl'.");

    /* Do whatever is necessary to receive \n for line endings on input from
       the console. A no-op on Unix. */
    set_lf_mode();

    if (o.listen)
        return ncat_listen_mode();
    else
        return ncat_connect_mode();
}

/* connect error handling and operations. */
static int ncat_connect_mode(void) {
    /*
     * allow/deny commands with connect make no sense. If you don't want to
     * connect to a host, don't try to.
     */
    if (o.allow || o.deny)
        bye("Invalid option combination: allow/deny with connect.");

    /* o.conn_limit with 'connect' doesn't make any sense. */
    if (o.conn_limit != -1)
        bye("Invalid option combination: `--max-conns' with connect.");

    /* connection brokering only applies in listen mode. */
    if (o.broker)
        bye("Invalid option combination: `--broker' with connect.");

    if (o.keepopen)
        bye("Invalid option combination: `--keep-open' with connect.");

    return ncat_connect();
}

static int ncat_listen_mode(void) {
    struct sockaddr_in *sin = (struct sockaddr_in *) &targetss;
#ifdef HAVE_IPV6
    static struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) &targetss;
#endif

    /* Can't 'listen' AND 'connect' to a proxy server at the same time. */
    if (httpconnect.ss_family != AF_UNSPEC || socksconnect.ss_family != AF_UNSPEC)
        bye("Invalid option combination: --proxy and -l.");

    if (o.idletimeout != 0)
        bye("An idle timeout only works in connect mode.");

    /* If a non-root user tries to bind to a privileged port, Exit. */
    if (o.portno < 1024 && !ncat_checkuid())
        bye("Attempted a non-root bind() to a port <1024.");

    if (o.broker && o.cmdexec != NULL)
        bye("Invalid option combination: --broker and -e.");

    /* Set the default maximum simultaneous TCP connection limit. */
    if (o.conn_limit == -1)
        o.conn_limit = DEFAULT_MAX_CONNS;

#ifndef WIN32
    /* See if the shell is executable before we get deep into this */
    if (o.shellexec && access("/bin/sh", X_OK) == -1)
        bye("/bin/sh is not executable, so `-c' won't work.");
#endif

    /* If we weren't given a specific address to listen on, we accept
     * any incoming connections
     */
    if (targetss.ss_family == AF_UNSPEC) {
        targetss.ss_family = o.af;

        if (o.af == AF_INET)
            sin->sin_addr.s_addr = INADDR_ANY;
#ifdef HAVE_IPV6
        else
            sin6->sin6_addr = in6addr_any;
#endif
    }

    /* Actually set our source address */
    srcaddr = targetss;

    if (srcaddr.ss_family == AF_INET)
        srcaddrlen = sizeof(struct sockaddr_in);
#ifdef HAVE_IPV6
    else
        srcaddrlen = sizeof(struct sockaddr_in6);
#endif

    /* If --broker was supplied, go into connection brokering mode. */
    if (o.broker)
        return ncat_broker();

    if (o.proxytype) {
        if (strcmp(o.proxytype, "http") == 0)
            o.httpserver = 1;
    }

    /* Fire the listen/select dispatcher for bog-standard listen operations. */
    return ncat_listen();
}
