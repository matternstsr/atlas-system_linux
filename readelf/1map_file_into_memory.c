#include "1-hreadelf.h"
#include <fcntl.h>    /* for open */
#include <unistd.h>   /* for close */
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>    /* for perror */

void *map_file_into_memory(const char *filename, size_t *filesize)
{
	int fd;
	struct stat st;
	void *m_file;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (NULL);
	}
	if (fstat(fd, &st) == -1)
	{
		perror("fstat");
		close(fd);
		return (NULL);
	}
	*filesize = st.st_size;
	m_file = mmap(NULL, *filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (m_file == MAP_FAILED)
	{
		perror("mmap");
		return (NULL);
	}
	return (m_file);
}
