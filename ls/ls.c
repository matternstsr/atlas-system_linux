#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int is_dot_or_dotdot(const char *name) {
	return name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
}

void bubble_sort(char **arr, int n) {
	int i, j;
	char *temp;
	for (i = 0; i < n - 1; i++) {
			for (j = 0; j < n - i - 1; j++) {
					int k = 0;
					while (arr[j][k] != '\0' && arr[j + 1][k] != '\0' && arr[j][k] == arr[j + 1][k]) {
							k++;
					}
					if (arr[j][k] > arr[j + 1][k]) {
							temp = arr[j];
							arr[j] = arr[j + 1];
							arr[j + 1] = temp;
					}
			}
	}
}

int main(void) {
	DIR *dir;
	struct dirent *entry;
	char *files[1000]; /* Assuming a maximum of 1000 files */
	int num_files = 0, i;

	dir = opendir(".");
	if (dir == NULL) {
			perror("opendir");
			exit(EXIT_FAILURE);
	}

	while ((entry = readdir(dir)) != NULL) {
			if (!is_dot_or_dotdot(entry->d_name)) {
					files[num_files++] = entry->d_name;
			}
	}

	closedir(dir);

	/* Sort filenames alphabetically */
	bubble_sort(files, num_files);

	/* Print sorted filenames */
	for (i = 0; i < num_files; i++) {
			printf("%s\n", files[i]);
	}

	return 0;
}
