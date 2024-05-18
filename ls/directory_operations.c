/* directory_operations.c */

#include "directory_reader.h"

/**
 * initDirectoryReader - Initializes a directory reader object with the specified path.
 * @reader: Pointer to a DirectoryReader structure to be initialized.
 * @path: Path of the directory to be read.
 *
 * Description: Does my stuff explained.
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
 * getNextEntry: Retr the next dir entry from the dir being read.
 * @reader: Pointer to a DirectoryReader structure.
 * Returns: Pointer to next directory entry. NULL if none or error.
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
 * forEachEntry: Iterates through each directory entry
 * and applies a specified item handler function to each entry.
 * @reader: Pointer to a DirectoryReader structure.
 * @itemHandler: Pointer to a function that handles each directory entry.
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
 * destroyDirectoryReader: Destroys a directory reader object,
 * closing the directory stream and freeing allocated memory.
 * @reader: Pointer to a DirectoryReader structure to be destroyed.
 */
void destroyDirectoryReader(DirectoryReader *reader)
{
	if (reader && reader->dir)
	{
	closedir(reader->dir);
	mattset(reader, 0, sizeof(*reader));
	}
}
