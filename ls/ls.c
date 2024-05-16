#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int is_dot_or_dotdot(const char *name) {
	return name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
}

int custom_strcomp(const char *s1, const char *s2) {
	int i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]) {
			i++;
	}
	return s1[i] - s2[i];
}

void bubble_sort(char **arr, int n) {
	int i, j;
	char *temp;
	for (i = 0; i < n - 1; i++) {
			for (j = 0; j < n - i - 1; j++) {
					if (custom_strcomp(arr[j], arr[j + 1]) > 0) {
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
					/* Allocate memory for the filename */
					files[num_files] = malloc(256 * sizeof(char));/* Assuming maximum filename length is 255 */
					if (files[num_files] == NULL) {
							perror("malloc");
							exit(EXIT_FAILURE);
					}

					/* Copy the filename character by character */
					int j = 0;
					while (entry->d_name[j] != '\0') {
							files[num_files][j] = entry->d_name[j];
							j++;
					}
					files[num_files][j] = '\0'; /* Null-terminate the string */
					num_files++;
			}
	}

	closedir(dir);

	/* Sort filenames alphabetically */
	bubble_sort(files, num_files);

	/* Print sorted filenames */
	for (i = 0; i < num_files; i++) {
			printf("%s\n", files[i]);
			free(files[i]); /* Free allocated memory for each filename */
	}

	return 0;
}
