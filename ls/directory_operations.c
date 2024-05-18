/* directory_operations.c */

#include "directory_reader.h"

/**
 * initDirectoryReader - Initializes a dir reader object with the spec path.
 * @reader: Pointer to a DirectoryReader structure to be initialized.
 * @path: Path of the directory to be read.
 *
 * Description: Initializes a dir reader object with the specified path.
 *              This function opens the dir specified by @path and sets up
 *              the DirectoryReader structure accordingly.
 *
 * Return: 0 on success, -1 if failed to open the directory.
*/

int initDirectoryReader(DirectoryReader *reader, const char *path)
{
	DIR *dir = opendir(path);

	if (!dir)
		return (-1);

	reader->dir = dir;
	reader->path = path;
	reader->current_entry = NULL;
	reader->finished = 0;
	return (0);
}

/**
 * destroyDirectoryReader - Destroys a directory reader object,
 * closing the directory stream and freeing allocated memory.
 * @reader: Pointer to a DirectoryReader structure to be destroyed.
 *
 * Description: This function destroys a directory reader object,
 * closing the directory stream and freeing allocated memory.
 */
void destroyDirectoryReader(DirectoryReader *reader)
{
	if (reader && reader->dir)
	{
	closedir(reader->dir);
	mattset(reader, 0, sizeof(*reader));
	}
}
