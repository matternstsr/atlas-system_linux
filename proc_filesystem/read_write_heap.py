#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

Looks thru the heap of a given process for a str and rep with another one.
"""

import sys
import os


def read_write_heap(pid, search_string, replace_string):
    try:
        # Open maps file to find heap range
        with open(f"/proc/{pid}/maps", "r") as maps_file:
            for line in maps_file:
                if "[heap]" in line:
                    heap_range = line.split()[0]
                    heap_start, heap_end = map(
    lambda x: int(x, 16), 
    heap_range.split("-")
)
                    break
            else:
                raise ValueError("Heap not found in process maps")

        # Open mem file to read from heap
        with open(f"/proc/{pid}/mem", "r+b") as mem_file:
            mem_file.seek(heap_start)
            heap_data = mem_file.read(heap_end - heap_start)

            # Check if search_string exists in heap_data
            search_bytes = search_string.encode()
            replace_bytes = replace_string.encode()
            search_index = heap_data.find(search_bytes)
            if search_index != -1:
                print(f"Found search string at offset {search_index} in heap")

                # Seek to the offset and write replace_string
                mem_file.seek(heap_start + search_index)
                mem_file.write(replace_bytes)
                print(
                        f"Successfully replaced '{search_string}' "
                        f"with '{replace_string}'"
                )
            else:
                print(f"Search string '{search_string}' not found in heap")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: read_write_heap.py pid search_string replace_string")
        sys.exit(1)

    pid = int(sys.argv[1])
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    read_write_heap(pid, search_string, replace_string)
