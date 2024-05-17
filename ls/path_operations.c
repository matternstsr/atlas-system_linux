/* path_operations.c */

#include "directory_reader.h"

/* Join directory path and entry name */
const char *joinPath(const char *dirpath, const char *entry_name)
{
static char PATH_BUF[4096];

snprintf(PATH_BUF, sizeof(PATH_BUF), "%s/%s", dirpath, entry_name);
return (PATH_BUF);
}
