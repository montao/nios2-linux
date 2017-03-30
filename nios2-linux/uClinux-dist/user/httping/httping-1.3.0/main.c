/* The GPL applies to this program.
  In addition, as a special exception, the copyright holders give
  permission to link the code of portions of this program with the
  OpenSSL library under certain conditions as described in each
  individual source file, and distribute linked combinations
  including the two.
  You must obey the GNU General Public License in all respects
  for all of the code used other than OpenSSL.  If you modify
  file(s) with this exception, you may extend this exception to your
  version of the file(s), but you are not obligated to do so.  If you
  do not wish to do so, delete this exception statement from your
  version.  If you delete this exception statement from all source
  files in the program, then also delete it here.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef NO_SSL
#include <openssl/ssl.h>
#include "mssl.h"
#endif
#include <arpa/inet.h>

#include "gen.h"
#include "http.h"
#include "io.h"
#include "str.h"
#include "mem.h"
#include "tcp.h"
#include "res.h"
#include "utils.h"
#include "error.h"

static volatile int stop = 0;

int quiet = 0;
char machine_readable = 0;
char nagios_mode = 0;

char last_error[ERROR_BUFFER_SIZE];

void version(void)
{
	fprintf(stderr, "HTTPing v" VERSION ", (C) 2003-2008 folkert@vanheusden.com\n");
#ifndef NO_SSL
	fprintf(stderr, "SSL support included\n");
#endif
}

void usage(void)
{
	fprintf(stderr, "\n-g url		url (e.g. -g http://localhost/)\n");
	fprintf(stderr, "-h hostname	hostname (e.g. localhost)\n");
	fprintf(stderr, "-p portnr	portnumber (e.g. 80)\n");
	fprintf(stderr, "-x host:port	hostname+portnumber of proxyserver\n");
	fprintf(stderr, "-c count	how many times to connect\n");
	fprintf(stderr, "-i interval	delay between each connect\n");
	fprintf(stderr, "-t timeout	timeout (default: 30s)\n");
	fprintf(stderr, "-s		show statuscodes\n");
	fprintf(stderr, "-S             split time in connect-time and processing time\n");
	fprintf(stderr, "-G		do a GET request instead of HEAD (read the\n");
	fprintf(stderr, "		contents of the page as well)\n");
	fprintf(stderr, "-b		show transfer speed in KB/s (use with -G)\n");
	fprintf(stderr, "-B		like -b but use compression if available\n");
	fprintf(stderr, "-L x		limit the amount of data transferred (for -b)\n");
	fprintf(stderr, "		to 'x' (in bytes)\n");
	fprintf(stderr, "-X		show the number of KB transferred (for -b)\n");
#ifndef NO_SSL
	fprintf(stderr, "-l		connect using SSL\n");
	fprintf(stderr, "-z		show fingerprint (SSL)\n");
#endif
	fprintf(stderr, "-f		flood connect (no delays)\n");
	fprintf(stderr, "-a		audible ping\n");
	fprintf(stderr, "-m		give machine parseable output (see\n");
	fprintf(stderr, "		also -o and -e)\n");
	fprintf(stderr, "-o rc,rc,...	what http results codes indicate 'ok'\n");
	fprintf(stderr, "		coma seperated WITHOUT spaces inbetween\n");
	fprintf(stderr, "		default is 200, use with -e\n");
	fprintf(stderr, "-e str		string to display when http result code\n");
	fprintf(stderr, "		doesn't match\n");
	fprintf(stderr, "-I str		use 'str' for the UserAgent header\n");
	fprintf(stderr, "-R str		use 'str' for the Referer header\n");
	fprintf(stderr, "-r		resolve hostname only once (usefull when\n");
	fprintf(stderr, "		pinging roundrobin DNS: also takes the first\n");
	fprintf(stderr, "		DNS lookup out of the loop so that the first\n");
	fprintf(stderr, "		measurement is also correct)\n");
	fprintf(stderr, "-n warn,crit	Nagios-mode: return 1 when avg. response time\n");
	fprintf(stderr, "		>= warn, 2 if >= crit, otherwhise return 0\n");
	fprintf(stderr, "-N x		Nagios mode 2: return 0 when all fine, 'x'\n");
	fprintf(stderr, "		when anything failes\n");
	fprintf(stderr, "-y ip[:port]   bind to ip-address (and thus interface) [/port]\n");
	fprintf(stderr, "-q		quiet, only returncode\n");
	fprintf(stderr, "-V		show the version\n\n");
}

void emit_error()
{
	if (!quiet && !machine_readable && !nagios_mode)
	{
		printf("%s", last_error);
	}

	if (!nagios_mode)
		last_error[0] = 0x00;
}

void handler(int sig)
{
	stop = 1;
}

int main(int argc, char *argv[])
{
	char *hostname = NULL;
	char *proxy = NULL, *proxyhost = NULL;
	int proxyport = 8080;
	int portnr = 80;
	char *get = NULL, *request;
	int req_len;
	int c;
	int count = -1, curncount = 0;
	int wait = 1;
	int audible = 0;
	int ok = 0, err = 0;
	double min = 999999999999999.0, avg = 0.0, max = 0.0;
	int timeout=30;
	char show_statuscodes = 0;
	char use_ssl = 0;
	SSL_CTX *client_ctx = NULL;
	char *ok_str = "200";
	char *err_str = "-1";
	char *useragent = NULL;
	char *referer = NULL;
	char *host;
	int port;
	struct sockaddr_in addr;
	char resolve_once = 0;
	char have_resolved = 0;
	double nagios_warn=0.0, nagios_crit=0.0;
	int nagios_exit_code = 2;
	double avg_httping_time = -1.0;
	int get_instead_of_head = 0;
	char *buffer;
	int page_size = sysconf(_SC_PAGESIZE);
	char show_Bps = 0, ask_compression = 0;
	int Bps_min = 1 << 30, Bps_max = 0;
	long long int Bps_avg = 0;
	int Bps_limit = -1;
	char show_bytes_xfer = 0, show_fp = 0;
	int fd = -1;
	SSL *ssl_h = NULL;
	BIO *s_bio;
	struct sockaddr_in bind_to;
	char bind_to_valid = 0;
	char split = 0;

	signal(SIGPIPE, SIG_IGN);

	if (page_size == -1)
		page_size = 4096;

	buffer = (char *)mymalloc(page_size, "receive buffer");

	while((c = getopt(argc, argv, "Sy:XL:bBg:h:p:c:i:Gx:t:o:e:falqsmV?I:R:rn:N:z")) != -1)
	{
		switch(c)
		{
			case 'S':
				split = 1;
				break;

			case 'y':
				{
					char *dummy = strchr(optarg, ':');

					bind_to_valid = 1;

					memset(&bind_to, 0x00, sizeof(bind_to));

					if (dummy)
					{
						bind_to.sin_port = htons(atoi(dummy + 1));
						*dummy = 0x00;
					}

					if (inet_aton(optarg, &bind_to.sin_addr) == 0)
					{
						error_exit("cannot convert ip address '%s' (for -y)\n", optarg);
					}
				}
				break;

			case 'z':
				show_fp = 1;
				break;

			case 'X':
				show_bytes_xfer = 1;
				break;

			case 'L':
				Bps_limit = atoi(optarg);
				break;

			case 'B':
				show_Bps = 1;
				ask_compression = 1;
				break;

			case 'b':
				show_Bps = 1;
				break;

			case 'e':
				err_str = optarg;
				break;

			case 'o':
				ok_str = optarg;
				break;

			case 'x':
				proxy = optarg;
				break;

			case 'g':
				get = optarg;
				break;

			case 'r':
				resolve_once = 1;
				break;

			case 'h':
				hostname = optarg;
				break;

			case 'p':
				portnr = atoi(optarg);
				break;

			case 'c':
				count = atoi(optarg);
				break;

			case 'i':
				wait = atoi(optarg);
				break;

			case 't':
				timeout = atoi(optarg);
				break;

			case 'I':
				useragent = optarg;
				break;

			case 'R':
				referer = optarg;
				break;

			case 'a':
				audible = 1;
				break;

			case 'f':
				wait = 0;
				break;

			case 'G':
				get_instead_of_head = 1;
				break;

#ifndef NO_SSL
			case 'l':
				use_ssl = 1;
				break;
#endif

			case 'm':
				machine_readable = 1;
				break;

			case 'q':
				quiet = 1;
				break;

			case 's':
				show_statuscodes = 1;
				break;

			case 'V':
				version();
				return 0;

			case 'n':
				{
					char *dummy = strchr(optarg, ',');
					if (nagios_mode) error_exit("-n and -N are mutual exclusive\n");
					nagios_mode = 1;
					if (!dummy)
						error_exit("-n: missing parameter\n");
					nagios_warn = atof(optarg);
					nagios_crit = atof(dummy + 1);
				} break;
			case 'N':
				if (nagios_mode) error_exit("-n and -N are mutual exclusive\n");
				nagios_mode = 2;
				nagios_exit_code = atoi(optarg);
				break;

			case '?':
			default:
				version();
				usage();
				return 1;
		}
	}

	last_error[0] = 0x00;

#ifndef NO_SSL
	if (use_ssl && portnr == 80)
		portnr = 443;
#endif

	if (!get_instead_of_head && show_Bps)
		error_exit("-b/-B can only be used when also using -G\n");

	if (get != NULL && hostname == NULL)
	{
		char *slash, *colon;
		char *getcopy = mystrdup(get, "get request");
		char *http_string = "http://";
		int http_string_len = 7;

#ifndef NO_SSL
		if (use_ssl)
		{
			http_string_len = 8;
			http_string = "https://";
		}
#endif

		if (strncasecmp(getcopy, http_string, http_string_len) != 0)
		{
			fprintf(stderr, "'%s' is a strange URL\n", getcopy);
			fprintf(stderr, "Expected: %s...\n", http_string);
			if (strncasecmp(getcopy, "https://", 8) == 0)
				fprintf(stderr, "Did you forget to add the '-l' switch to the httping commandline?\n");
			return 2;
		}

		slash = strchr(&getcopy[http_string_len], '/');
		if (slash)
			*slash = 0x00;

		colon = strchr(&getcopy[http_string_len], ':');
		if (colon)
		{
			*colon = 0x00;
			portnr = atoi(colon + 1);
		}

		hostname = &getcopy[http_string_len];
	}

	if (hostname == NULL)
	{
		usage();
		error_exit("No hostname/getrequest given\n");
	}

	if (get == NULL)
	{
#ifndef NO_SSL
		if (use_ssl)
		{
			get = mymalloc(8 /* http:// */ + strlen(hostname) + 1 /* colon */ + 5 /* portnr */ + 1 /* / */ + 1 /* 0x00 */, "get");
			sprintf(get, "https://%s:%d/", hostname, portnr);
		}
		else
		{
#endif
			get = mymalloc(7 /* http:// */ + strlen(hostname) + 1 /* colon */ + 5 /* portnr */ + 1 /* / */ + 1 /* 0x00 */, "get");
			sprintf(get, "http://%s:%d/", hostname, portnr);
#ifndef NO_SSL
		}
#endif
	}

	if (proxy)
	{
		char *dummy = strchr(proxy, ':');
		proxyhost = proxy;
		if (dummy)
		{
			*dummy=0x00;
			proxyport = atoi(dummy + 1);
		}

		if (!quiet && !nagios_mode)
			fprintf(stderr, "Using proxyserver: %s:%d\n", proxyhost, proxyport);
	}

#ifndef NO_SSL
	if (use_ssl)
	{
		client_ctx = initialize_ctx();
		if (!client_ctx)
		{
			snprintf(last_error, ERROR_BUFFER_SIZE, "problem creating SSL context\n");
			goto error_exit;
		}
	}
#endif

	request = mymalloc(strlen(get) + 4096, "request");
	if (proxyhost)
		sprintf(request, "%s %s HTTP/1.0\r\n", get_instead_of_head?"GET":"HEAD", get);
	else
	{
		char *dummy = get, *slash;
		if (strncasecmp(dummy, "http://", 7) == 0)
			dummy += 7;
		else if (strncasecmp(dummy, "https://", 7) == 0)
			dummy += 8;

		slash = strchr(dummy, '/');
		if (slash)
			sprintf(request, "%s %s HTTP/1.0\r\n", get_instead_of_head?"GET":"HEAD", slash);
		else
			sprintf(request, "%s / HTTP/1.0\r\n", get_instead_of_head?"GET":"HEAD");
	}
	if (useragent)
		sprintf(&request[strlen(request)], "User-Agent: %s\r\n", useragent);
	else
		sprintf(&request[strlen(request)], "User-Agent: HTTPing v" VERSION "\r\n");
	sprintf(&request[strlen(request)], "Host: %s\r\n", hostname);
	if (referer)
		sprintf(&request[strlen(request)], "Referer: %s\r\n", referer);
	if (ask_compression)
		sprintf(&request[strlen(request)], "Accept-Encoding: gzip,deflate\r\n");
	strcat(request, "\r\n");
	req_len = strlen(request);

	if (!quiet && !machine_readable && !nagios_mode)
		printf("PING %s:%d (%s):\n", hostname, portnr, get);

	signal(SIGINT, handler);
	signal(SIGTERM, handler);

	timeout *= 1000;	/* change to ms */

	host = proxyhost?proxyhost:hostname;
	port = proxyhost?proxyport:portnr;

	if (resolve_once)
	{
		memset(&addr, 0x00, sizeof(addr));

		if (resolve_host(host, &addr) == -1)
		{
			err++;
			emit_error();
			have_resolved = 1;
		}
	}

	while((curncount < count || count == -1) && stop == 0)
	{
		double ms;
		double dstart, dend, dafter_connect = 0.0;
		char *reply;
		int Bps = 0;
		char is_compressed = 0;
		long long int bytes_transferred = 0;

		dstart = get_ts();

		for(;;)
		{
			char *fp = NULL;
			int rc;
			char *sc = NULL, *scdummy = NULL;

			curncount++;

			if (!resolve_once || (resolve_once == 1 && have_resolved == 0))
			{
				memset(&addr, 0x00, sizeof(addr));

				if (resolve_host(host, &addr) == -1)
				{
					err++;
					emit_error();
					break;
				}

				have_resolved = 1;
			}

			fd = connect_to(bind_to_valid?(struct sockaddr *)&bind_to:NULL, &addr, port, timeout);

			if (fd == -3)	/* ^C pressed */
				break;

			if (fd < 0)
				emit_error();

			if (fd >= 0)
			{
				/* set socket to low latency */
				if (set_tcp_low_latency(fd) == -1)
					break;

				/* set fd blocking */
				if (set_fd_blocking(fd) == -1)
					break;

#ifndef NO_SSL
				if (use_ssl)
				{
					int rc;

					rc = connect_ssl(fd, client_ctx, &ssl_h, &s_bio, timeout);
					if (rc != 0)
					{
						close(fd);
						fd = rc;
					}
				}
#endif
			}
			if (split)
				dafter_connect = get_ts();

			if (fd < 0)
			{
				if (fd == -2)
					snprintf(last_error, ERROR_BUFFER_SIZE, "timeout connecting to host\n");

				emit_error();

				err++;

				break;
			}

#ifndef NO_SSL
			if (use_ssl)
				rc = WRITE_SSL(ssl_h, request, req_len);
			else
#endif
				rc = mywrite(fd, request, req_len, timeout);
			if (rc != req_len)
			{
				if (rc == -1)
					snprintf(last_error, ERROR_BUFFER_SIZE, "error sending request to host\n");
				else if (rc == -2)
					snprintf(last_error, ERROR_BUFFER_SIZE, "timeout sending to host\n");
				else if (rc == -3)
				{/* ^C */}
				else if (rc == 0)
					snprintf(last_error, ERROR_BUFFER_SIZE, "connection prematurely closed by peer\n");

				emit_error();

				close(fd);
				err++;

				break;
			}

			rc = get_HTTP_headers(fd, ssl_h, &reply, timeout);

			if ((show_statuscodes || machine_readable) && reply != NULL)
			{
				/* statuscode is in first line behind
				 * 'HTTP/1.x'
				 */
				char *dummy = strchr(reply, ' ');

				if (dummy)
				{
					sc = strdup(dummy + 1);

					/* lines are normally terminated with a
					 * CR/LF
					 */
					dummy = strchr(sc, '\r');
					if (dummy)
						*dummy = 0x00;
					dummy = strchr(sc, '\n');
					if (dummy)
						*dummy = 0x00;
				}
			}

			if (ask_compression && reply != NULL)
			{
				char *encoding = strstr(reply, "\nContent-Encoding:");
				if (encoding)
				{
					char *dummy = strchr(encoding + 1, '\n');
					if (dummy) *dummy = 0x00;

					if (strstr(encoding, "gzip") == 0 || strstr(encoding, "deflate") == 0)
					{
						is_compressed = 1;
					}
				}
			}

			free(reply);

			if (rc < 0)
			{
				if (rc == RC_SHORTREAD)
					snprintf(last_error, ERROR_BUFFER_SIZE, "error receiving reply from host\n");
				else if (rc == RC_TIMEOUT)
					snprintf(last_error, ERROR_BUFFER_SIZE, "timeout receiving reply from host\n");

				emit_error();

				close(fd);
				err++;

				break;
			}

			ok++;

			if (get_instead_of_head)
			{
				double dl_start = get_ts(), dl_end;

				for(;;)
				{
					int rc = read(fd, buffer, page_size);

					if (rc == -1)
					{
						if (errno != EINTR && errno != EAGAIN)
							error_exit("read failed");
					}
					else if (rc == 0)
						break;

					bytes_transferred += rc;

					if (Bps_limit != -1 && bytes_transferred >= Bps_limit)
						break;
				}

				dl_end = get_ts();

				Bps = bytes_transferred / max(dl_end - dl_start, 0.000001);
				Bps_min = min(Bps_min, Bps);
				Bps_max = max(Bps_max, Bps);
				Bps_avg += Bps;
			}

			dend = get_ts();

#ifndef NO_SSL
			if (use_ssl)
			{
				if (show_fp && ssl_h != NULL)
				{
					fp = get_fingerprint(ssl_h);
				}

				if (close_ssl_connection(ssl_h, fd) == -1)
				{
					snprintf(last_error, ERROR_BUFFER_SIZE, "error shutting down ssl\n");
					emit_error();
				}

				SSL_free(ssl_h);
			}
#endif
			close(fd);

			ms = (dend - dstart) * 1000.0;
			avg += ms;
			min = min > ms ? ms : min;
			max = max < ms ? ms : max;

			if (machine_readable)
			{
				if (sc)
				{
					char *dummy = strchr(sc, ' ');

					if (dummy) *dummy = 0x00;

					if (strstr(ok_str, sc))
					{
						printf("%f", ms);
					}
					else
					{
						printf("%s", err_str);
					}

					if (show_statuscodes)
						printf(" %s", sc);
				}
				else
				{
					printf("%s", err_str);
				}
				if(audible)
					putchar('\a');
				printf("\n");
			}
			else if (!quiet && !nagios_mode)
			{
				if (split)
				{
					printf("connected to %s:%d, seq=%d time=%.2f+%.2f=%.2f ms %s", hostname, portnr, curncount-1, (dafter_connect - dstart) * 1000.0, (dend - dafter_connect) * 1000.0, ms, sc?sc:"");
				}
				else
				{
					printf("connected to %s:%d, seq=%d time=%.2f ms %s", hostname, portnr, curncount-1, ms, sc?sc:"");
				}

				if (show_Bps)
				{
					printf(" %dKB/s", Bps / 1024);
					if (show_bytes_xfer)
						printf(" %dKB", (int)(bytes_transferred / 1024));
					if (ask_compression)
					{
						printf(" (");
						if (!is_compressed)
							printf("not ");
						printf("compressed)");
					}
				}

				if (use_ssl && show_fp && fp != NULL)
				{
					printf(" %s", fp);
					free(fp);
				}
				if(audible)
					putchar('\a');
				printf("\n");
			}

			if (show_statuscodes && ok_str != NULL && sc != NULL)
			{
				scdummy = strchr(sc, ' ');
				if (scdummy) *scdummy = 0x00;

				if (strstr(ok_str, sc) == NULL)
				{
					ok--;
					err++;
				}
			}

			free(sc);

			break;
		}

		if (curncount != count && !stop)
			sleep(wait);
	}

	avg_httping_time = avg / (double)ok;

	if (!quiet && !machine_readable && !nagios_mode)
	{
		printf("--- %s ping statistics ---\n", get);

		if (count == -1)
			printf("%d connects, %d ok, %3.2f%% failed\n", curncount, ok, (((double)err) / ((double)curncount)) * 100.0);
		else
			printf("%d connects, %d ok, %3.2f%% failed\n", curncount, ok, (((double)err) / ((double)count)) * 100.0);

		if (ok > 0)
		{
			printf("round-trip min/avg/max = %.1f/%.1f/%.1f ms\n", min, avg_httping_time, max);

			if (show_Bps)
				printf("Transfer speed: min/avg/max = %d/%d/%d KB\n", Bps_min / 1024, (int)(Bps_avg / ok) / 1024, Bps_max / 1024);
		}
	}

error_exit:
	if (nagios_mode == 1)
	{
		if (ok == 0)
		{
			printf("CRITICAL - connecting failed: %s", last_error);
			return 2;
		}
		else if (avg_httping_time >= nagios_crit)
		{
			printf("CRITICAL - average httping-time is %.1f\n", avg_httping_time);
			return 2;
		}
		else if (avg_httping_time >= nagios_warn)
		{
			printf("WARNING - average httping-time is %.1f\n", avg_httping_time);
			return 1;
		}

		printf("OK - average httping-time is %.1f (%s)\n", avg_httping_time, last_error);
	}
	else if (nagios_mode == 2)
	{
		if (ok && last_error[0] == 0x00)
		{
			printf("OK - all fine, avg httping time is %.1f\n", avg_httping_time);
			return 0;
		}

		printf("%s: - failed: %s", nagios_exit_code == 1?"WARNING":(nagios_exit_code == 2?"CRITICAL":"ERROR"), last_error);
		return nagios_exit_code;
	}

	if (ok)
		return 0;
	else
		return 127;
}
