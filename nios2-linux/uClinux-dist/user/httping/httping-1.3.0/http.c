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
#include <string.h>
#include <unistd.h>
#ifndef NO_SSL
#include <openssl/ssl.h>
#include "mssl.h"
#endif

#include "gen.h"
#include "mem.h"
#include "http.h"
#include "io.h"
#include "str.h"
#include "utils.h"

int get_HTTP_headers(int socket_h, SSL *ssl_h, char **headers, int timeout)
{
	char *buffer = mymalloc(128, "http header");
	int len_in=0, len=128;
	char byte=-1, prev_byte=-2;
	int rc = RC_OK;

	*headers = NULL;

	for(;;)
	{
		int rrc;

		if (byte != 13)
			prev_byte = byte;

#ifndef NO_SSL
		if (ssl_h)
			rrc = READ_SSL(ssl_h, &byte, 1);
		else
#endif
			rrc = myread(socket_h, &byte, 1, timeout);
		if (rrc == 0 || rrc == RC_SHORTREAD)	/* socket closed before request was read? */
		{
			rc = RC_SHORTREAD;
			buffer[len_in] = 0x00;
			break;
		}
		else if (rrc == RC_TIMEOUT)		/* timeout */
		{
			return RC_TIMEOUT;
		}

		if (len_in >= (len-1))
		{
			len <<= 1;
			buffer = (char *)myrealloc(buffer, len, "http reply");
		}

		buffer[len_in++] = byte;

		if (prev_byte == 10 && byte == 10)
		{
			buffer[len_in]=0x00;
			break;	
		}
	}

	*headers = buffer;

	return rc;
}

char * get_lf_terminated_str(char *pnt)
{
	char *end = strchr(pnt, '\n');
	int len = strlen(pnt);

	if (end)
		return strndup(pnt, (end-pnt)+1);
	else
		return strndup(pnt, len);
}
