/* path_operations.c */

#include "directory_reader.h"

/*
 * joinPath: Join directory path and entry name.
 * @dirpath: Pointer to the directory path string.
 * @entry_name: Pointer to the entry name string.
 * Returns: Pointer to a static buffer containing the joined path.
 */
const char *joinPath(const char *dirpath, const char *entry_name)
{
static char PATH_BUF[4096];

snprintf(PATH_BUF, sizeof(PATH_BUF), "%s/%s", dirpath, entry_name);
return (PATH_BUF);
}
