#!/usr/bin/python3
"""
Finds and replaces a specified string
in a provided PID heap memory
"""

if __name__ == "__main__":
    from sys import argv

    if len(argv) != 4:
        exit("Unsupported  number of arguments")
    pid = int(argv[1])
    search_str = argv[2].encode()
    replace_str = argv[3].encode()
    try:
        mem_map = open(f"/proc/{pid}/maps", "r")
    except FileNotFoundError as error:
        exit(error)
    for line in mem_map:
        if line.endswith("[heap]\n"):
            heap_start, heap_end = \
                [int(x, 16) for x in line.split(" ")[0].split("-")]
    print("Target memory location found")
    mem_map.close()
    try:
        heap_mem = open(f"/proc/{pid}/mem", "r+b")
    except FileNotFoundError as error:
        exit(error)
    heap_mem.seek(heap_start)
    mem = heap_mem.read(heap_end - heap_start)
    str_at = mem.find(search_str) + heap_start
    print(f"{search_str}:{str_at}")
    if str_at > -1:
        print(f"Target Located at {hex(str_at)}")
        if len(search_str) > len(replace_str):
            heap_mem.seek(str_at + len(replace_str))
            heap_mem.write(b'\0')
        else:
            heap_mem.seek(str_at)
            heap_mem.write(replace_str)
        print("Target Replaced")
    else:
        print("Unable to locate Target")
    heap_mem.close()
    print("Mission Complete")
    exit()
