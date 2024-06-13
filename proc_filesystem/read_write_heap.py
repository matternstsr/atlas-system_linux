#!/usr/bin/python3
"""
Python - Python - /proc filesystem - 0. Hack the VM

Looks thru the heap of a given process for a str and rep with another one.
"""

import sys
import os
import re
import subprocess
import time


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

    Args:
        maps_content (str): Contents of the '/proc/[pid]/maps' file.

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

    proc_c = subprocess.Popen(['./main_c'])
    time.sleep(1)

    add_in_memory = None

    maps_filename = "/proc/{}/maps".format(pid)
    mem_filename = "/proc/{}/mem".format(pid)

    proc_py = None

    def read_value_in_memory(path, addr, l):
        value_read = None
        # write the new string
        if add_in_memory is not None:
            try:
                mem_file = open(path, 'rb')
                mem_file.seek(addr)
                value_read = mem_file.read(l)
            except Exception as e:
                print(e)
            finally:
                mem_file.close()
        return value_read

    try:
        with open(maps_filename, mode="r") as maps_file:
            while True:
                line = maps_file.readline()
                if not line:
                    break
                sline = line.split(' ')

                try:
                    # check if we found the heap
                    if (sline[-1][:-1] == "[heap]"):

                        # parse line
                        addr = sline[0]
                        perm = sline[1]
                        offset = sline[2]
                        device = sline[3]
                        inode = sline[4]
                        pathname = sline[-1][:-1]

                        # get start and end of mem
                        addr = addr.split("-")
                        addr_start = int(addr[0], 16)
                        addr_end = int(addr[1], 16)

                        # open and read mem
                        try:
                            mem_file = open(mem_filename, 'rb')
                            # read heap
                            mem_file.seek(addr_start)
                            heap = mem_file.read(addr_end - addr_start)
                            # find string
                            try:
                                add_in_memory = heap.index(bytes(search_string, "ASCII"))
                                add_in_memory += addr_start
                            except Exception as e:
                                print(e)
                            mem_file.close()
                        except Exception as e:
                            print(e)

                        break
                except Exception as e:
                    print(e)

        previous_value = read_value_in_memory(mem_filename, add_in_memory, len(search_string))

        # start py prog
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
        proc_c.kill()
        if proc_py is not None:
            proc_py.kill()
