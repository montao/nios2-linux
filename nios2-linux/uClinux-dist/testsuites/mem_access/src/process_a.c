/* malloc a region in this process_a and set it all to char 's';
 * record the address to file /tmp/mem_addr, so process_b can get it easily;
 * generate file /tmp/data for further test.
 */


#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main ()
{
    char *buffer;
    int i=0, ret=0;
    FILE* fp;
    size_t buf_size = 16;

    /* allocate a buffer */
    buffer = malloc(buf_size);
    if (!buffer) {
        printf("FAIL: malloc(%zu) failed\n", buf_size);
        exit(1);
    }

    /* create a file to restore data */
    memset(buffer, 'd', buf_size);
    fp = fopen("/tmp/data", "w+");
    if (!fp) {
        fprintf(stderr, "error opening file: %s\n", strerror(errno));
        exit(1);
    }
    ret = fprintf (fp, "%.16s\n", (int)buffer);
    if (ret <= 0) {
        printf("error writinging file: %d characters are written\n", ret);
        exit(1);
    }
    fclose (fp);

    /* set buffer content to char 's' */
    memset(buffer, 's', buf_size);

    /* show buffer content */
    for(i=0;i<buf_size;i++)
        printf ("%c", buffer[i]);
    printf ("\n");

    /* create a file to restore buffer address */
    fp = fopen("/tmp/mem_addr", "w+");
    if (!fp) {
        fprintf(stderr, "error opening file: %s\n", strerror(errno));
        exit(1);
    }
    ret = fprintf (fp, "%d\n", (int)buffer);
    if (ret <= 0) {
        printf("error writinging file: %d characters are written\n", ret);
        exit(1);
    }
    fclose (fp);

    /* wait for another process to access buffer in this process */
    printf ("buffer address is 0x%08x(%d)\nNow sleep...\n", (int)buffer, (int)buffer);
    sleep (30);

    printf ("Wake up now...\n");

    /* check buffer content again */
    for(i=0;i<buf_size;i++)
        printf ("%c", buffer[i]);
    printf ("\n");

    return 0;
}
