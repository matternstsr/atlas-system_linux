/* directory_operations.c */

#include "directory_reader.h"

/**
 * initDirectoryReader - Initializes a dir reader object with the spec path.
 * @reader: Pointer to a DirectoryReader structure to be initialized.
 * @path: Path of the directory to be read.
 *
 * Description: Initializes a directory reader object with the specified path.
 *
 * Returns: 0 on success, -1 if failed to open the directory.
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
 * getNextEntry - Retrieves the next directory entry
 * from the directory being read.
 * @reader: Pointer to a DirectoryReader structure.
 *
 * Description: This function retrieves the next directory
 * entry from the directory being read.
 *
 * Returns: Pointer to the next directory entry. NULL if none or error.
 */
struct dirent *getNextEntry(DirectoryReader *reader)
{
	struct dirent *next_entry = readdir(reader->dir);

	if (next_entry)
		reader->current_entry = next_entry;
		else
	{
	reader->finished = 1;
	reader->current_entry = NULL;
	}
	return (next_entry);
}

/**
 * forEachEntry - Iterates through each directory entry and
 * applies a specified item handler function to each entry.
 * @reader: Pointer to a DirectoryReader structure.
 * @itemHandler: Pointer to a function that handles each directory entry.
 *
 * Description: This function iterates through each directory
 * entry and applies a specified item handler function to each entry.
 *
 * Returns: The number of directory entries iterated.
 */
int forEachEntry(DirectoryReader *reader,
								int (*itemHandler)(DirectoryReader *))
{
	int entry_count = 0;

	while (getNextEntry(reader))
	{
		if (itemHandler(reader) == -1)
		{
		/* Handle error */
		fprintf(stderr, "Error handling directory entry\n");
		}
		++entry_count;
	}
	return (entry_count);
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
