#include "1-hreadelf.h"

const char *convert_flags_to_string(uint64_t flags)
{
    static char flag_string[16];  /* Adjust size as needed */
    const char *flag_chars = "WAXMSILGTEopO";
    int position = 0, i;

    for (i = 0; i < 13; ++i) {
        if (flags & (1UL << i))
            flag_string[position++] = flag_chars[i];
    }

    flag_string[position] = '\0';

     /* Handle case where no flags are set */
    if (position == 0)
        return " ";  /* or NULL or any appropriate default */

    return flag_string;
}
