#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

Looks thru the heap of a given process for a str and rep with another one.
"""

import sys
import os
import re


def read_maps(pid):
    """
    Read the contents of the '/proc/[pid]/maps' file for the given process ID.

    Args:
        pid (int): The process ID.

    Returns:
        str: The contents of the maps file.
    """
    maps_file_path = "/proc/{}/maps".format(pid)
    try:
        with open(maps_file_path, 'r') as maps_file:
            return maps_file.read()
    except FileNotFoundError:
        print("Error: Process with PID {} not found.".format(pid))
        sys.exit(1)


def find_heap_address(maps_content):
    """
    Find the start and end addresses of the heap from the maps content.

    Args: s of the '/proc/[pid]/maps' file.

    Returns:
        tuple: A tuple containing the start and end addresses of the heap.
    """
    heap_regex = r'([0-9A-Fa-f]+)-([0-9A-Fa-f]+) \[heap\]'
    match = re.search(heap_regex, maps_content)
    if match:
        return (int(match.group(1), 16), int(match.group(2), 16))
    else:
        print("Error: Heap address not found in process maps.")
        sys.exit(1)


def replace_string_in_heap(pid, search_string, replace_string):
    """
    Find and replace a string in the heap of a running process.

    Args:
        pid (int): The process ID.
        search_string (str): The string to search for in the heap.
        replace_string (str): The string to replace the found string with.
    """
    maps_content = read_maps(pid)
    start, end = find_heap_address(maps_content)

    with open("/proc/{}/mem".format(pid), 'rb+') as mem_file:
        mem_file.seek(start)
        heap_data = mem_file.read(end - start)

        if bytes(search_string, 'ASCII') not in heap_data:
            print("Error: string not found heap with PID {}.".format(pid))
            sys.exit(1)

        mem_file.seek(start + heap_data.index(bytes(search_string, 'ASCII')))
        mem_file.write(bytes(replace_string, 'ASCII'))

if __name__ == "__main__":
    """
    Entry point of the script.
    """
    # Check if the correct number of arguments is provided
    if len(sys.argv) != 4:
        print("Usage: {} pid search_string replace_string".format(sys.argv[0]))
        sys.exit(1)

    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    # Check if the provided PID is a positive integer
    if not pid.isdigit():
        print("Error: PID must be a positive integer.")
        sys.exit(1)

    replace_string_in_heap(int(pid), search_string, replace_string)
