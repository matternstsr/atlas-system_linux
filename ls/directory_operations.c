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
 * getNextEntry - Retrieves the next directory entry
 * from the directory being read.
 * @reader: Pointer to a DirectoryReader structure.
 *
 * Description: This function retrieves the next directory
 * entry from the directory being read.
 *
 * Return: Pointer to the next directory entry. NULL if none or error.
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
 * Return: The number of directory entries iterated.
 */

int forEachEntry(DirectoryReader *reader,
                                int (*itemHandler)(DirectoryReader *))
{
	int entry_count = 0;
	int capacity = INITIAL_CAPACITY;
	struct dirent **entries;
	struct dirent **new_entries;

	entries = malloc(capacity * sizeof(struct dirent *));
	if (entries == NULL)
	{
		/* Handle memory allocation failure */
		fprintf(stderr, "Error: Failed to allocate memory for directory entries.\n");
		return -1;
	}

	/* Collect directory entries into an array */
	while (getNextEntry(reader))
	{
		if (entry_count >= capacity)
		{
			/* Resize the array if it's full */
			capacity *= 2;
			new_entries = realloc(entries, capacity * sizeof(struct dirent *));
			if (new_entries == NULL)
			{
				/* Handle memory reallocation failure */
				free(entries);
				fprintf(stderr, "Error: Failed to reallocate memory for directory entries.\n");
				return -1;
			}
			entries = new_entries;
		}
		entries[entry_count++] = reader->current_entry;
	}

	/* Sort the array of directory entries */
	mattsort(entries, entry_count);

	/* Iterate over sorted entries and process them */
	int i;
	for (i = 0; i < entry_count; ++i)
	{
		reader->current_entry = entries[i];
		if (itemHandler(reader) == -1)
		{
			/* Handle error */
			fprintf(stderr, "Error handling directory entry\n");
		}
	}

	/* Free dynamically allocated memory */
	free(entries);

	return entry_count;
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
