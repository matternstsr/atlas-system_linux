#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int is_dot_or_dotdot(const char *name) {
	return strcmp(name, ".") == 0 || strcmp(name, "..") == 0;
}

int compare(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

int main(void) {
	DIR *dir;
	struct dirent *entry;
	struct stat statbuf;
	char *files[1000];

	dir = opendir(".");
	if (dir == NULL) {
			perror("opendir");
			exit(EXIT_FAILURE);
	}

	int num_files = 0;

	while ((entry = readdir(dir)) != NULL) {
			if (!is_dot_or_dotdot(entry->d_name)) {
					files[num_files++] = strdup(entry->d_name);
			}
	}

	closedir(dir);

	qsort(files, num_files, sizeof(char *), compare);

	for (int i = 0; i < num_files; i++) {
			printf("%s\n", files[i]);
			free(files[i]);
	}

	return 0;
}
