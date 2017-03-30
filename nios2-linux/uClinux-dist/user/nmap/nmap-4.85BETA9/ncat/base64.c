#include "base64.h"
#include "nbase.h"

static int b64enc_internal(const unsigned char *data, int len, char *dest)
{
    /* base64 alphabet, taken from rfc3548 */
    char *b64alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *buf = dest;

    /* Encode three bytes per iteration ala rfc3548. */
    while (len >= 3) {
        buf[0] = b64alpha[(data[0] >> 2) & 0x3f];
        buf[1] = b64alpha[((data[0] << 4) & 0x30) | ((data[1] >> 4) & 0xf)];
        buf[2] = b64alpha[((data[1] << 2) & 0x3c) | ((data[2] >> 6) & 0x3)];
        buf[3] = b64alpha[data[2] & 0x3f];
        data += 3;
        buf += 4;
        len -= 3;
    }

    /* Pad the remaining bytes. len is 0, 1, or 2 here. */
    if (len > 0) {
        buf[0] = b64alpha[(data[0] >> 2) & 0x3f];
        if (len > 1) {
            buf[1] = b64alpha[((data[0] << 4) & 0x30) | ((data[1] >> 4) & 0xf)];
            buf[2] = b64alpha[(data[1] << 2) & 0x3c];
        } else {
            buf[1] = b64alpha[(data[0] << 4) & 0x30];
            buf[2] = '=';
        }
        buf[3] = '=';
        buf += 4;
    }

    /*
     * As mentioned in rfc3548, we need to be careful about
     * how we null terminate and handle embedded null-termination.
     */
    *buf = '\0';

    return (buf - dest);
}

/* Take in plain text and encode into base64. */
char *b64enc(const unsigned char *data, int len)
{
    char *dest;

    /* malloc enough space to do something useful */
    dest = (char*)safe_malloc(4 * len / 3 + 4);

    dest[0] = '\0';

    /* Call internal function to base64 encode data */
    b64enc_internal(data, len, dest);

    return (dest);
}
