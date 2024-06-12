#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks through the heap of a given process for string and replaces it with
another one.
"""

import sys
import os

def find_and_replace(pid, search_string, replace_string):
	"""
	Finds and replaces a specified string in the heap memory of a process identified by PID.

	Args:
		pid (int): Process ID of the target process.
		search_string (str): String to search for in the heap memory.
		replace_string (str): String to replace the found string with.

	Returns:
		None
	"""
	try:
		with open(f"/proc/{pid}/maps", "r") as mem_map:
			for line in mem_map:
				if line.endswith("[heap]\n"):
					heap_start, heap_end = \
						[int(x, 16) for x in line.split(" ")[0].split("-")]
	except FileNotFoundError as error:
		sys.exit(error)
	print("Target memory location found")

	try:
		with open(f"/proc/{pid}/mem", "r+b") as heap_mem:
			heap_mem.seek(heap_start)
			mem = heap_mem.read(heap_end - heap_start)
			str_at = mem.find(search_string.encode()) + heap_start
			print(f"{search_string}:{str_at}")
			if str_at > -1:
				print(f"Target Located at {hex(str_at)}")
				if len(search_string) > len(replace_string):
					heap_mem.seek(str_at + len(replace_string))
					heap_mem.write(b'\0')
				else:
					heap_mem.seek(str_at)
					heap_mem.write(replace_string.encode())
				print("Target Replaced")
			else:
				print("Unable to locate Target")
	except FileNotFoundError as error:
		sys.exit(error)
	print("Mission Complete")

if __name__ == "__main__":
	if len(sys.argv) != 4:
		sys.exit("Unsupported number of arguments")
	pid = int(sys.argv[1])
	search_str = sys.argv[2]
	replace_str = sys.argv[3]
	find_and_replace(pid, search_str, replace_str)
