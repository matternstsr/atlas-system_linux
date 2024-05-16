#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int is_dot_or_dotdot(const char *name) {
	return name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
}

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
			if (!is_dot_or_dotdot(entry->d_name)) {
					printf("%s\n", entry->d_name);
			}
	}

	closedir(dir);

	return 0;
}
