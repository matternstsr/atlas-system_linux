# Read Write Heap

This script allows you to find and replace a string in the heap of a running process.

## Usage

```
read_write_heap.py pid search_string replace_string
```

- `pid`: The process ID of the running process.
- `search_string`: The string to search for in the heap.
- `replace_string`: The string to replace the found string with.

## Requirements

- Python 3.4.3
- Ubuntu 14.04 LTS
- PEP 8 Style
- Access to the `/proc` filesystem

## Installation

No installation is required. Simply download the `read_write_heap.py` script and ensure it is executable.

## Example

To replace the string "hello" with "world" in the heap of a process with PID 1234:

```
./read_write_heap.py 1234 hello world
```

## Notes

- This script only works on Linux systems due to its reliance on the `/proc` filesystem.
- Ensure that you have the necessary permissions to read and write to the process memory.
- Use with caution, as modifying the memory of a running process can have unintended consequences.
