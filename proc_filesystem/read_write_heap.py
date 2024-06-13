#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks thru the heap of a given process for a str and rep with another one.
"""

import sys
import os

def replace_string_in_heap(pid, search_string, replace_string):
    try:
        # Open maps file
        maps_file = open("/proc/{}/maps".format(pid), 'r')
        for line in maps_file:
            fields = line.split()
            if fields[-1] == "[heap]":
                # Found the heap region
                start_addr = int(fields[0].split("-")[0], 16)
                end_addr = int(fields[0].split("-")[1], 16)
                # Open mem file
                mem_file = open("/proc/{}/mem".format(pid), 'rb+')
                mem_file.seek(start_addr)
                heap_data = mem_file.read(end_addr - start_addr)
                # Search for the string
                offset = heap_data.find(search_string.encode())
                if offset != -1:
                    mem_file.seek(start_addr + offset)
                    mem_file.write(replace_string.encode() + b'\0')
                    print("String replaced successfully.")
                    mem_file.close()
                    maps_file.close()
                    return
        print("String not found in heap.")
        maps_file.close()
    except Exception as e:
        print("Error:", e)

def main():
    if len(sys.argv) != 4:
        print("Usage: {} pid search_string replace_string".format(sys.argv[0]))
        sys.exit(1)

    try:
        pid = int(sys.argv[1])
    except ValueError:
        print("Error: pid must be an integer.")
        sys.exit(1)

    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    replace_string_in_heap(pid, search_string, replace_string)

if __name__ == "__main__":
    main()
