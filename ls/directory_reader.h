#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

/** HEADERS **/

#include <sys/types.h>/* opendir, readdir, closedir, lstat */
#include <dirent.h>/* opendir, readdir, closedir */
#include <stdio.h>/* printf, sprintf, fprintf */
#include <sys/stat.h>/* lstat */
#include <unistd.h>
#include <errno.h>/* errno */
#include <time.h>/* Header for ctime */
#include <stdbool.h>/* For booleans */
#include <pwd.h>/* For getpwuid */
#include <grp.h>/* For group_grabber */
#include <stdlib.h>/* For malloc, free */

/** STRUCTS **/

/**
 * struct file_link_s - file linked list node
 * @name: file name
 * @dir_name: dir name
 * @info: stat struct with file info
 * @next: next
 * @prev: prev
 **/
typedef struct file_link_s
{
	char *name;
	char *dir_name;
	struct stat *info;
	struct file_link_s *next;
	struct file_link_s *prev;
} file_node_t;

/**
 * struct LsFlags - flags struct
 * @printer: printer (changed to a long printer if -l is present)
 * @one_per_line: -1
 * @dot: -a
 * @dot_alt: -A ('.' and '..' ommitted)
 * @reversed: -r
 * @sort_by_size: -S
 * @sort_by_time: -t
 * @recursive: -R
 * @print_dir_name: if true, print directory name before printing list
 * @max_hard_links: max hard links
 * @max_size: max size
 * @max_strlen: max strlen
 **/
typedef struct LsFlags
{
	void (*printer)(file_node_t *, struct LsFlags *);
	bool one_per_line;
	bool dot;
	bool dot_alt;
	bool reversed;
	bool sort_by_size;
	bool sort_by_time;
	bool recursive;
	bool print_dir_name;
	int max_hard_links;
	int max_size;
	int max_strlen;
} ls_flag_t;

typedef void (*print_t)(file_node_t *, struct LsFlags *);

/**
 * struct dir_ops_s - directory node
 * @dir_name: dir name
 * @list: list
 * @error_code: error_code
 * @size: size in bytes
 * @next: next
 * @prev: prev
 **/
typedef struct dir_ops_s
{
	char *dir_name;
	file_node_t *list;
	int error_code;
	int size;
	struct dir_ops_s *next;
	struct dir_ops_s *prev;
} dir_ops_t;

#define ISLOWER(x) ((x) >= 'a' && (x) <= 'z')
#define ISUPPER(x) ((x) >= 'A' && (x) <= 'Z')

#define IS_PARENT_DIR(x) (str_len(x) == 2 && x[0] == '.' && x[1] == '.')
#define IS_CWD(x) (str_len(x) == 1 && x[0] == '.')
#define IS_PATH(x) (char_search(x, '/') != NULL)
#define IS_HIDDEN(x) (x[0] == '.')
#define should_print(x) (!IS_HIDDEN(x) || IS_PATH(x) || flags->dot || \
						(flags->dot_alt && !IS_CWD(x) && !IS_PARENT_DIR(x)))

/* Function Prototypes */

/* Long format helper functions (in string_getters.c) */
void perm_log(char *buffer, mode_t mode);
void group_grabber(char *buffer, gid_t group_id);
void user_grabber(char *buffer, uid_t user_id);
void last_modified(char *buffer, time_t time_val);
char file_type(mode_t mode);

/* Set flags helper function (in main.c) */
int flag_interpreter(char *arg, ls_flag_t *flags);

/* Linked list creation functions (in node_makers.c) */
dir_ops_t *add_subdirectories(dir_ops_t *dir, ls_flag_t *flags);
void manage_subdirectories(dir_ops_t **head, dir_ops_t *dir, ls_flag_t *flags);
int add_directory(char *name, DIR *stream, dir_ops_t **head);
int add_file(char *file_name, char *dir_name, file_node_t **head);
file_node_t *file_maker(char *name, char *dir_name, struct stat *info);
bool should_open_directory(file_node_t *dir, ls_flag_t *flags);

/* Alphabetization logic (in first_alphabetical_string.c) */
char *first_alphabetical_string(char *s1, char *s2);

/* Printing functions */
int print_dirs(dir_ops_t **head, ls_flag_t *flags, print_t printer);
void print_long_list(file_node_t *file_list, ls_flag_t *flags);
void print_short_list(file_node_t *file_list, ls_flag_t *flags);
int error_message_printing(char *name);

/* Custom string functions (in string_helpers.c) */
char *char_search(char *str, char c);
char *string_copy(char *dest, char *src);
char *string_dup(char *str);
int str_len(char *str);

/* file sorting functions (in sorters.c)*/
file_node_t *recent_file_sort(file_node_t *head);
int compare_recent(file_node_t *a, file_node_t *b);
void swap_files(file_node_t *a, file_node_t *b);
file_node_t *file_size_sort_checker(file_node_t *head);
file_node_t *file_size_sort(file_node_t *head);

/* directory sorting functions (in dir_sorters.c) */
dir_ops_t *sort_dir_size(dir_ops_t *head);
int comp_dir_size(dir_ops_t *a, dir_ops_t *b);
void swap_dir_pos(dir_ops_t *a, dir_ops_t *b);

#endif /* DIRECTORY_READER_H */