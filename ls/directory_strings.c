#include "directory_reader.h"
/**
 * user_grabber - Retrieves the username associated with a given user ID.
 * @buffer: Buffer to store the username.
 * @user_id: User ID.
 **/
void user_grabber(char *buffer, uid_t user_id)
{
    struct passwd *user_info = getpwuid(user_id);

    if (user_info && user_info->pw_name && user_info->pw_name[0])
        string_copy(buffer, user_info->pw_name);
    else
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

    time_string = ctime(&time_val);
    for (i = 0, j = 4; colon_count < 2; i++, j++)
    {
        buffer[i] = time_string[j];
        if (buffer[i] == ':')
            colon_count++;
    }
    buffer[i - 1] = '\0';
}

/**
 * group_grabber - Retrieves the group name associated with a given group ID.
 * @buffer: Buffer to store the group name.
 * @group_id: Group ID.
 **/
void group_grabber(char *buffer, gid_t group_id)
{
    struct group *group_info = getgrgid(group_id);
    if (group_info && group_info->gr_name && group_info->gr_name[0])
        string_copy(buffer, group_info->gr_name);
    else
        sprintf(buffer, "%u", group_id);
}

/**
 * perm_log - Creates a string representing file permissions (for 'ls -l' format).
 * @buffer: Buffer to store the permission string.
 * @mode: File mode, used to extract permissions.
 **/
void perm_log(char *buffer, mode_t mode)
{
    buffer[0] = file_type(mode);
    buffer[1] = mode & S_IRUSR ? 'r' : '-';
    buffer[2] = mode & S_IWUSR ? 'w' : '-';
    buffer[3] = mode & S_IXUSR ? 'x' : '-';
    buffer[4] = mode & S_IRGRP ? 'r' : '-';
    buffer[5] = mode & S_IWGRP ? 'w' : '-';
    buffer[6] = mode & S_IXGRP ? 'x' : '-';
    buffer[7] = mode & S_IROTH ? 'r' : '-';
    buffer[8] = mode & S_IWOTH ? 'w' : '-';
    buffer[9] = mode & S_IXOTH ? 'x' : '-';
    buffer[10] = '\0';
}

/**
 * file_type - Determines the type of a file and returns a corresponding character.
 * @mode: File mode.
 * Return: Character representing the file type.
 **/
char file_type(mode_t mode)
{
    if (S_ISREG(mode))
        return ('-');
    if (S_ISDIR(mode))
        return ('d');
    if (S_ISCHR(mode))
        return ('c');
    if (S_ISBLK(mode))
        return ('b');
    if (S_ISFIFO(mode))
        return ('p');
    if (S_ISLNK(mode))
        return ('l');
    if (S_ISSOCK(mode))
        return ('s');
    return ('-');
}
