/* read the memory address in process_a from file /tmp/mem_addr;
 * open an existing file /tmp/data;
 * read file content to memory in process_a, it should be fail;
 * write the file from memory in process_a, it should be fail.
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main ()
{
    char *buffer;
    int ret=0, fd;
    FILE* fp;
    size_t buf_size = 16;

    /* read address from a certain file */
    fp = fopen("/tmp/mem_addr", "r");
    if (!fp) {
        fprintf(stderr, "error opening file: %s\n", strerror(errno));
        exit(1);
    }
    fscanf (fp, "%d\n", &buffer);
    printf ("buffer address is 0x%08x(%d)\n", (int)buffer, (int)buffer);

    /* Open data file and read/write from it */
    fd = open("/tmp/data", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "error opening file: %s\n", strerror(errno));
        exit(1);
    }

    ret = read(fd, buffer, buf_size);
    if (ret == -1) {
        printf ("PASS: read file content to memory in another process, failed as expected\n");
    } else {
        printf ("FAIL: read file content to memory in another process, return value %d\n", ret);
    }

    ret = write(fd, buffer, buf_size);
    if (ret == -1) {
        printf ("PASS: write file from memory in another process, failed as expected\n");
    } else {
        printf ("FAIL: write file from memory in another process, return value %d\n", ret);
    }

    return 0;
}
