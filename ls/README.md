# Directory Operations

## Introduction
This project provides a set of functions for performing operations on directories in C. It includes functionalities like reading directory contents, handling directory entries, and obtaining information about directory entries.

## File Structure
- **directory_reader.h**: Header file defining structures and function prototypes for directory reading operations.
- **directory_operations.c**: Implements functions for initializing directory readers, iterating through directory entries, and destroying directory readers.

## Usage
To use this project, include the necessary header file `directory_reader.h` in your C code and link against the compiled object files of the provided source files (`path_operations.c`, `entry_operations.c`, `directory_operations.c`).

### Example Usage:
```c
#include "directory_reader.h"
#include <stdio.h>

int main() {
    DirectoryReader reader;
    if (initDirectoryReader(&reader, "/path/to/directory") == -1) {
        fprintf(stderr, "Error opening directory\n");
        return -1;
    }

    printf("Contents of directory:\n");
    forEachEntry(&reader, printEntryName);

    destroyDirectoryReader(&reader);
    return 0;
}
```

## Functions

### `initDirectoryReader`
- **Description**: Initializes a directory reader with the specified directory path.
- **Parameters**:
  - `reader`: Pointer to a `DirectoryReader` structure.
  - `path`: Path of the directory to be read.
- **Returns**: `0` on success, `-1` on failure.

### `forEachEntry`
- **Description**: Iterates through the directory entries and applies the specified item handler function to each entry.
- **Parameters**:
  - `reader`: Pointer to a `DirectoryReader` structure.
  - `itemHandler`: Pointer to the item handler function.
- **Returns**: Number of directory entries processed.

### `destroyDirectoryReader`
- **Description**: Destroys the directory reader and releases associated resources.
- **Parameters**:
  - `reader`: Pointer to a `DirectoryReader` structure.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.




# C - ls

## Concepts
For this project, we expect you to look at this concept:

- [EYNTK] Create your own ls

## Resources
Read or watch:

- Everything you need to know to write your own ls
- man ls

## Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

**General**
- How does ls work?
- What are all the functions used by ls?

## Requirements
**General**
- Allowed editors: vi, vim, emacs
- All your files will be compiled on Ubuntu 14.04 LTS
- Your C programs and functions will be compiled with gcc 4.8.4 using the flags -Wall -Werror -Wextra and -pedantic
- All your files should end with a new line
- A README.md file, at the root of the folder of the project is mandatory
- Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
- No more than 5 functions per file
- All your header files should be include guarded
- Valgrind should show 0 memory leak and 0 error
- Unless specified otherwise, your program must have the exact same output as ls as well as the exact same error output - except for alignment.

**Allowed Functions and System Calls**
- opendir (man 3 opendir)
- readdir (man 3 readdir)
- closedir (man 3 closedir)
- exit (man 3 exit)
- free (man 3 free)
- lstat (man 2 lstat)
- malloc (man 3 malloc)
- perror (man 3 perror)
- write (man 2 write)
- printf (man 3 printf)
- sprintf (man 3 sprintf)
- fprintf (man 3 fprintf)
- readlink (man 2 readlink)
- ctime (man 3 ctime)
- getpwuid (man 3 getpwuid)
- getgrgid (man 3 getgrgid)
- errno (man 3 errno)

## More Info
**Compilation**  
Your program will be compiled this way:  
```c
gcc -Wall -Werror -Wextra -pedantic *.c -o hls
```

Every task depends on the previous ones. We strongly encourage you to read the entire project and think about the way you are going to design your entire ls before starting. (This is actually something you should do all the time :)).

**Testing**  
Your program should give the same result as the real ls.

```c
$ ls test
abc  BCD  file  file2  file3  folder1  folder2  folder3
$ ./hls test
abc  BCD  file  file2  file3  folder1  folder2  folder3
$ ls -1
abc
BCD
file
file2
file3
folder1
folder2
folder3
hls
$ ./hls -1
abc
BCD
file
file2
file3
folder1
folder2
folder3
hls
$
```

# Tasks

## Task 0: Let's start with something simple! *(mandatory)*

Create a program that lists the content of the current directory.

Usage: `hls`

```c
wilfried@~$ ls
abc  file  File2  file3  folder1  Folder2  folder3  hls
wilfried@~$ ./hls
abc  file  File2  file3  folder1  Folder2  folder3  hls
wilfried@~$

```

## Task 1: Maybe some parameters? *(mandatory)*

Your program should now take one or more file or directory names as parameters.

Usage: `hls [FILE]...`

Errors must be printed the same way than `ls` does:

- In stderr
- Starting by `<program>:` (`<program>` being `argv[0]`)
- `No such file or directory`, `Permission denied`, etc…
- Don’t forget the exit value

```c
wilfried@~$ ls test
abc  file  File2  file3  folder1  Folder2  folder3
wilfried@~$ ./hls test
abc  file  File2  file3  folder1  Folder2  folder3
wilfried@~$ ./hls test/folder1/ test
test:
abc  file  File2  file3  folder1  Folder2  folder3

test/folder1/:
folder1-1  folder1-2  folder1-3
/simple_ls$ ./hls test/random
./hls: cannot access test/random: No such file or directory
wilfried@~$
```

## Task 2: What about options? *(mandatory)*

Implement the `-1` option.

Usage: `hls [-1] [FILE]...`

For the rest of the project, an option will be identified by a command-line argument starting with the character `-` (like `ls`).

```c
wilfried@~$ ls test -1
abc
file
File2
file3
folder1
Folder2
folder3
wilfried@~$ ./hls test -1
abc
file
File2
file3
folder1
Folder2
folder3
wilfried@~$
```

## Task 3: Hidden files *(mandatory)*

Implement the `-a` option.

Usage: `hls [-a1] [FILE]...`

```c
wilfried@~$ ls -a test
.  ..  abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
wilfried@~$ ./hls -a test
.  ..  abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
wilfried@~$ ./hls -a test/folder1
.  ..  folder1-1  folder1-2  folder1-3
wilfried@~$  
```

## Task 4: Almost all *(mandatory)*

Implement the `-A` option.

Usage: `hls [-A] [FILE]...`

```c
wilfried@~$ ls -A test
abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
wilfried@~$ ./hls -A test
abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
wilfried@~$ ./hls -A test/folder1
folder1-1  folder1-2  folder1-3
wilfried@~$
```


## Task 5: More details *(mandatory)*

Implement the `-l` option.

Usage: `hls [-l] [FILE]...`

```c
wilfried@~$ ls -l test
total 12
-rw-rw-r-- 1 wilfried wilfried    0 May  8 00:19 abc
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 file
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 File2
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 file3
drwxrwxr-x 5 wilfried wilfried 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 wilfried wilfried 4096 May  7 15:36 Folder2
drwxrwxr-x 3 wilfried wilfried 4096 May  7 15:37 folder3
wilfried@~$ ./hls -l test
-rw-rw-r-- 1 wilfried wilfried    0 May  8 00:19 abc
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 file
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 File2
-rw-rw-r-- 1 wilfried wilfried    0 Apr 18 14:04 file3
drwxrwxr-x 5 wilfried wilfried 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 wilfried wilfried 4096 May  7 15:36 Folder2
drwxrwxr-x 3 wilfried wilfried 4096 May  7 15:37 folder3
wilfried@~$
```

You might notice the `total 12` in the first line of `ls -l`. You can ignore this value for this exercise.
