/* directory_op_entries.c */

#include "directory_reader.h"

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