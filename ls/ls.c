#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

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
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
						printf("%s\n", entry->d_name);
				}
		}

		closedir(dir);

		return 0;
}
