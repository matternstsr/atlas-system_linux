#include "1-hreadelf.h"
#include <fcntl.h>    /* for open */
#include <unistd.h>   /* for close */
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>    /* for perror */

void *map_file_into_memory(const char *filename, size_t *filesize)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return NULL;
    }

    *filesize = st.st_size;

    void *mapped_file = mmap(NULL, *filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);  /* Close file descriptor after mmap regardless of success
 */
    if (mapped_file == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    return mapped_file;
}