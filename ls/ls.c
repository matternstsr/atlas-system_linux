#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);

    return 0;
}
