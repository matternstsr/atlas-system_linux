/* directory_op_entries.c */

#include "directory_reader.h"

/**
 * getNextEntry - Retrieves the next directory entry
 * from the directory being read.
 * @reader: Pointer to a DirectoryReader structure.
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
 * entry, collects them into an array, sorts the array, and then
 * applies a specified item handler function to each entry.
 *
 * Return: The number of directory entries iterated.
 */
int forEachEntry(DirectoryReader *reader,
														int (*itemHandler)(DirectoryReader *))
{
	int entry_count = 0;                /* Cntr for the # of dir entries */
	int capacity = INITIAL_CAPACITY;    /* Initial cap of the dir entry array */
	struct dirent **entries;            /* Array to hold dir entries */
	struct dirent **new_entries;        /* Pointer for reallocated array */
	int i;                              /* Loop variable */

	entries = malloc(capacity * sizeof(struct dirent *));
	if (entries == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory for dir entries.\n");
		return (-1);
	}
	while (getNextEntry(reader))
	{
		if (entry_count >= capacity)
		{
			capacity *= 2;
			new_entries = realloc(entries, capacity * sizeof(struct dirent *));
			if (new_entries == NULL)
			{
				free(entries);
				fprintf(stderr, "Error: Failed to reallocate mem for dir entries.\n");
				return (-1);
			}
			entries = new_entries;
		}
		entries[entry_count++] = reader->current_entry;
	}
	mattsort(entries, entry_count);
	for (i = 0; i < entry_count; ++i)
	{
		reader->current_entry = entries[i];
		if (itemHandler(reader) == -1)
			fprintf(stderr, "Error handling directory entry\n");
	}
	free(entries);
	return (entry_count);
}