#!/usr/bin/python3

"""
read_write_heap.py - A script to find and replace a string in the heap of a running process.

Usage: read_write_heap.py pid search_string replace_string

Arguments:
    pid (int): The process ID of the running process.
    search_string (str): The string to search for in the heap (ASCII).
    replace_string (str): The string to replace the found string with (ASCII).

Behavior:
    This script locates the heap of the specified process using the /proc/[pid]/maps file.
    Then, it reads the memory of the process from /proc/[pid]/mem, searches for the search_string in the heap,
    replaces it with replace_string, and prints an appropriate message indicating success or failure.
    If any error occurs during the process, it prints an error message and exits with status code 1.
"""

import sys

def find_and_replace_in_heap(pid, search_string, replace_string):
    """
    Finds and replaces a string in the heap of a running process.

    Args:
        pid (int): The process ID of the running process.
        search_string (str): The string to search for in the heap (ASCII).
        replace_string (str): The string to replace the found string with (ASCII).

    Returns:
        None

    Raises:
        ValueError: If the heap is not found in the maps file or the input is invalid.
        OSError: If there's an error accessing the files in /proc/[pid].
    """
    # Validate input
    try:
        pid = int(pid)
    except ValueError:
        print("Error: Invalid process ID.")
        sys.exit(1)

    if not search_string or not isinstance(search_string, str):
        print("Error: Invalid search string.")
        sys.exit(1)

    if not isinstance(replace_string, str):
        print("Error: Invalid replace string.")
        sys.exit(1)

    # File paths
    maps_file_path = "/proc/{}/maps".format(pid)
    mem_file_path = "/proc/{}/mem".format(pid)

    try:
        # Read the maps file to find the heap address range
        with open(maps_file_path, 'r') as maps_file:
            for line in maps_file:
                if "[heap]" in line:
                    heap_info = line.split()
                    break
            else:
                raise ValueError("Heap not found in maps file.")

        # Extract heap address range
        heap_start, heap_end = map(lambda x: int(x, 16), heap_info[0].split('-'))

        # Open the memory file
        with open(mem_file_path, 'rb+') as mem_file:
            # Seek to the start of the heap
            mem_file.seek(heap_start)
            # Read the heap contents
            heap_data = mem_file.read(heap_end - heap_start)

            # Search for the search_string in the heap
            search_bytes = search_string.encode('ASCII')
            offset = heap_data.find(search_bytes)
            if offset != -1:
                # Replace the search_string with replace_string
                mem_file.seek(heap_start + offset)
                mem_file.write(replace_string.encode('ASCII'))
                print("SUCCESS!")
            else:
                print("String not found in heap.")
    except OSError as e:
        print("Error accessing /proc/{}: {}".format(pid, e))
        sys.exit(1)
    except ValueError as e:
        print("Error:", e)
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: {} pid search_string replace_string".format(sys.argv[0]))
        sys.exit(1)

    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    find_and_replace_in_heap(pid, search_string, replace_string)

