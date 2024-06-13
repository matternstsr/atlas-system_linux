#!/usr/bin/python3

"""
read_write_heap.py - Python script to find and replace a string in the heap of a running process.

Usage:
    read_write_heap.py pid search_string replace_string

Arguments:
    pid               PID of the running process.
    search_string     String to search for in the heap (ASCII).
    replace_string    String to replace the search string with (ASCII).

Description:
    This script reads the heap memory of a specified process using the /proc filesystem.
    It searches for a given string in the heap and replaces it with another string.

    The script first locates the heap memory range of the process by reading the /proc/[pid]/maps file.
    Then, it reads the heap memory from /proc/[pid]/mem, searches for the specified search string,
    and replaces it with the specified replace string.

    If the search string is found and replaced successfully, it prints "SUCCESS!".
    If the search string is not found, it prints "FAIL!".

    If any error occurs during the process, it prints an error message and exits with status code 1.

Example:
    read_write_heap.py 1234 "Hello" "Hi"
"""

import subprocess
import time
import sys
import os

def read_value_in_memory(path, addr, l):
    """
    Read a value from memory at a specified address.

    Args:
        path: File path to read memory from.
        addr: Address to read from.
        l: Length of the value to read.

    Returns:
        The value read from memory.
    """
    value_read = None
    try:
        mem_file = open(path, 'rb')
        mem_file.seek(addr)
        value_read = mem_file.read(l)
        mem_file.close()
    except Exception as e:
        print(e)
    return value_read

def read_write_heap(pid, search_string, replace_string):
    """
    Read and modify the heap of a specified process.

    Args:
        pid: PID of the process.
        search_string: String to search for in the heap.
        replace_string: String to replace the search string with.
    """
    add_in_memory = None
    proc_py = None
    try:
        maps_filename = f"/proc/{pid}/maps"
        mem_filename = f"/proc/{pid}/mem"

        with open(maps_filename, mode="r") as maps_file:
            for line in maps_file:
                if "[heap]" in line:
                    sline = line.split(' ')
                    addr = sline[0].split("-")
                    addr_start = int(addr[0], 16)
                    addr_end = int(addr[1], 16)

                    try:
                        mem_file = open(mem_filename, 'rb')
                        mem_file.seek(addr_start)
                        heap = mem_file.read(addr_end - addr_start)
                        add_in_memory = heap.find(bytes(search_string, "ASCII"))
                        if add_in_memory != -1:
                            add_in_memory += addr_start
                        mem_file.close()
                    except Exception as e:
                        print(e)
                    break

        if add_in_memory is not None:
            previous_value = read_value_in_memory(mem_filename, add_in_memory, len(search_string))
            proc_py = subprocess.Popen(['./read_write_heap.py', str(pid), search_string, replace_string])
            time.sleep(2)
            new_value = read_value_in_memory(mem_filename, add_in_memory, len(search_string))

            if previous_value != new_value:
                print("SUCCESS!")
            else:
                print("FAIL!")
    except Exception as e:
        print(e)
    finally:
        if proc_py is not None:
            proc_py.kill()
        else:
            print("FAIL!")  # If proc_py is not defined, consider the operation as failed

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: read_write_heap.py pid search_string replace_string")
        sys.exit(1)

    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    read_write_heap(pid, search_string, replace_string)
