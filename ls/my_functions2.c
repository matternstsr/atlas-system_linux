#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

/* Custom error function */
const char *mattError(int errnum) {
    switch (errnum) {
        case ENOENT:
            return "Directory not found";
        case EACCES:
            return "Access denied";
        default:
            return "Unknown error";
    }
}