/* atlas-system_linux/ls/directory_strings.c */

#include "directory_reader.h"

/**
 * user_grabber - Retrieves the username associated with a given user ID.
 * @buffer: Buffer to store the username.
 * @user_id: User ID.
 **/
void user_grabber(char *buffer, uid_t user_id)
{
    /* Retrieve user information based on user ID */
    struct passwd *user_info = getpwuid(user_id);

    /* Check if user information exists and if username is not empty */
    if (user_info && user_info->pw_name && user_info->pw_name[0])
        /* Copy username to the buffer */
        string_copy(buffer, user_info->pw_name);
    else
        /* If no username found, convert user ID to string and store */
        sprintf(buffer, "%u", user_id);
}


/**
 * last_modified - Generates a time string suitable for 'ls -l' format.
 * @buffer: Buffer to store the time string.
 * @time_val: Time value.
 **/
void last_modified(char *buffer, time_t time_val)
{
    char *time_string;
    int i, j, colon_count = 0;

    /* Get time string */
    time_string = ctime(&time_val);
    /* Copy hours and minutes to buffer */
    for (i = 0, j = 4; colon_count < 2; i++, j++)
    {
        buffer[i] = time_string[j];
        if (buffer[i] == ':')
            colon_count++;
    }
    /* Null-terminate buffer */
    buffer[i - 1] = '\0';
}



/**
 * group_grabber - Retrieves the group name associated with a given group ID.
 * @buffer: Buffer to store the group name or ID.
 * @group_id: Group ID.
 **/
void group_grabber(char *buffer, gid_t group_id)
{
    /* Retrieve group information based on group ID */
    struct group *group_info = getgrgid(group_id);
    if (group_info != NULL) {
        /* Check if group name is available */
        if (group_info->gr_name != NULL && group_info->gr_name[0] != '\0') {
            string_copy(buffer, group_info->gr_name); /* Copy group name to buffer */
            return;
        }
    }
    
    /* If group name not found or group_info is NULL, fallback to group ID */
    sprintf(buffer, "%u", (unsigned int)group_id); /* Store group ID as string */
}


/**
 * perm_log - Creates a string representing file permissions (for 'ls -l' format).
 * @buffer: Buffer to store the permission string.
 * @mode: File mode, used to extract permissions.
 **/
void perm_log(char *buffer, mode_t mode)
{
    /* Store file type */
    buffer[0] = file_type(mode);
    /* User permissions */
    buffer[1] = mode & S_IRUSR ? 'r' : '-';
    buffer[2] = mode & S_IWUSR ? 'w' : '-';
    buffer[3] = mode & S_IXUSR ? 'x' : '-';
    /* Group permissions */
    buffer[4] = mode & S_IRGRP ? 'r' : '-';
    buffer[5] = mode & S_IWGRP ? 'w' : '-';
    buffer[6] = mode & S_IXGRP ? 'x' : '-';
    /* Others permissions */
    buffer[7] = mode & S_IROTH ? 'r' : '-';
    buffer[8] = mode & S_IWOTH ? 'w' : '-';
    buffer[9] = mode & S_IXOTH ? 'x' : '-';
    /* Null-terminate the string */
    buffer[10] = '\0';
}


/**
 * file_type - Determines the type of a file and returns a corresponding character.
 * @mode: File mode.
 * Return: Character representing the file type.
 **/
char file_type(mode_t mode)
{
    /* Check if regular file */
    if (S_ISREG(mode))
        return '-';
    /* Check if directory */
    if (S_ISDIR(mode))
        return 'd';
    /* Check if character device */
    if (S_ISCHR(mode))
        return 'c';
    /* Check if block device */
    if (S_ISBLK(mode))
        return 'b';
    /* Check if FIFO (named pipe) */
    if (S_ISFIFO(mode))
        return 'p';
    /* Check if symbolic link */
    if (S_ISLNK(mode))
        return 'l';
    /* Check if socket */
    if (S_ISSOCK(mode))
        return 's';
    /* If none of the above, return '-' */
    return '-';
}
