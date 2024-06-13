#!/usr/bin/python3
"""
Write a script that finds a string in the heap of a running process, and
replaces it.
- Usage: read_write_heap.py pid search_string replace_string
-- where pid is the pid of the running process
-- and strings are ASCII
- The script should look only in the heap of the process
- Output: you can print whatever you think is interesting
- On usage error, print an error message on stdout and exit with status code 1
"""

if __name__ == "__main__":
    import os
    import re
    from sys import argv

    if len(argv) != 4 or len(argv[2]) < len(argv[3]):
        exit("Usage: ./read_write_heap.py pid search_string replace_string")
    pid = int(argv[1])
    try:
        os.kill(pid, 0)
    except OSError:
        exit("PID not found")
    print(f"[~] PID : {pid}")
    search_str = argv[2].encode()
    replace_str = argv[3].encode()
    try:
        maps = open(f"/proc/{pid}/maps", "r")
    except FileNotFoundError as err:
        exit(err)
    for line in maps:
        if re.search(r"^.*\[heap\]$", line):
            print("[~] HEAP LOCATED")
            heap, perms, off, dev, inode, path_ = line.split(" ", maxsplit=5)
            path = path_.strip()
            print(f"\tPATHNAME: {path}")
            print(f"\tADDRESS RANGE: {heap}")
            print(f"\tPERMISSIONS: {perms}")
            print(f"\tOFFSET: {off}")
            print(f"\tDEV: {dev}")
            print(f"\tINODE: {inode}")
            break
    maps.close()
    heap_begin, heap_end = [int(addr, 16) for addr in heap.split("-")]
    print(f"[~] HEAP BEGIN: [{hex(heap_begin)}] | HEAP END: [{hex(heap_end)}]")
    try:
        mem = open(f"/proc/{pid}/mem", "r+b")
    except FileNotFoundError as err:
        exit(err)
    mem.seek(heap_begin)
    address = mem.read(heap_end - heap_begin).find(search_str) + heap_begin
    if address >= heap_begin:
        print(f"[~] \"{argv[2]}\" FOUND AT {hex(address)}")
        mem.seek(address)
        mem.write(replace_str)
        if len(replace_str) < len(search_str):
            mem.seek(address + len(replace_str))
            mem.write(b'\0')
        print(f"[~] \"{argv[3]}\" WRITTEN AT {hex(address)}")
    else:
        print(f"[~] \"{argv[2]}\" NOT FOUND")
    mem.close()
    exit()