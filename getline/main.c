// Test case 1: Redefine READ_SIZE to 1 and count the number of calls to read
char *filename = "_getline.c";
char *file_content = "Test content";
int expected_read_calls = 4;

// Test case 2: Redefine READ_SIZE to 4 and count the number of calls to read
char *filename = "_getline.c";
char *file_content = "Test content";
int expected_read_calls = 1;
