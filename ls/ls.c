#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int is_dot_or_dotdot(const char *name) 
{
	return name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
}

int main(void)
{
	DIR *dir;
	struct dirent *entry;
	struct stat statbuf;

	dir = opendir(".");
	if (dir == NULL) {
			perror("opendir");
			exit(EXIT_FAILURE);
	}

	while ((entry = readdir(dir)) != NULL) {
			if (!is_dot_or_dotdot(entry->d_name)) {
					if (stat(entry->d_name, &statbuf) == -1) {
							perror("stat");
							exit(EXIT_FAILURE);
					}
					if (S_ISDIR(statbuf.st_mode)) {
							printf("%s:\n", entry->d_name);
							DIR *subdir = opendir(entry->d_name);
							if (subdir == NULL) {
									perror("opendir");
									exit(EXIT_FAILURE);
							}
							struct dirent *subentry;
							while ((subentry = readdir(subdir)) != NULL) {
									if (!is_dot_or_dotdot(subentry->d_name)) {
											printf("%s\n", subentry->d_name);
									}
							}
							closedir(subdir);
					} else {
							printf("%s\n", entry->d_name);
					}
			}
	}

	closedir(dir);

	return 0;
}
