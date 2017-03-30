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

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gen.h"
#include "res.h"

extern char last_error[];

int resolve_host(char *host, struct sockaddr_in *addr)
{
        struct hostent *hostdnsentries;

        hostdnsentries = gethostbyname(host);
        if (hostdnsentries == NULL)
        {
		switch(h_errno)
		{
		case HOST_NOT_FOUND:
			snprintf(last_error, ERROR_BUFFER_SIZE, "The specified host is unknown.\n");
			break;

		case NO_ADDRESS:
			snprintf(last_error, ERROR_BUFFER_SIZE, "The requested name is valid but does not have an IP address.\n");
			break;

		case NO_RECOVERY:
			snprintf(last_error, ERROR_BUFFER_SIZE, "A non-recoverable name server error occurred.\n");
			break;

		case TRY_AGAIN:
			snprintf(last_error, ERROR_BUFFER_SIZE, "A temporary error occurred on an authoritative name server. Try again later.\n");
			break;

		default:
			snprintf(last_error, ERROR_BUFFER_SIZE, "Could not resolve %s for an unknown reason (%d)\n", host, h_errno);
		}

                return -1;
        }

	/* create address structure */
        addr -> sin_family = hostdnsentries -> h_addrtype;
	addr -> sin_addr = incopy(hostdnsentries -> h_addr_list[0]);

        return 0;
}
