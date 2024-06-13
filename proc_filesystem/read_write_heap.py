#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks thru the heap of a given process for a str and rep with another one.
"""

import os
import re
from sys import argv

def main():
	# Check for correct command-line arguments
	if len(argv) != 4 or len(argv[2]) < len(argv[3]):
			exit("Usage: ./read_write_heap.py pid search_string replace_string")

	# Extract PID from command-line arguments
	process_id = int(argv[1])

	# Check if PID exists
	try:
			os.kill(process_id, 0)
	except OSError:
			exit("PID not found")

	# Print process ID
	print(f"[~] Process ID: {process_id}")

	# Encode search and replace strings to bytes
	search_string = argv[2].encode()
	replace_string = argv[3].encode()

	try:
			with open(f"/proc/{process_id}/maps", "r") as memory_map:
					# Iterate through memory map to locate the heap
					for line in memory_map:
							if re.search(r"^.*\[heap\]$", line):
									print("[~] HEAP LOCATED")
									heap, permissions, offset, device, inode, path = line.split(" ", maxsplit=5)
									path = path.strip()
									print(f"\tPATHNAME: {path}")
									print(f"\tADDRESS RANGE: {heap}")
									print(f"\tPERMISSIONS: {permissions}")
									print(f"\tOFFSET: {offset}")
									print(f"\tDEVICE: {device}")
									print(f"\tINODE: {inode}")
									break

	except FileNotFoundError as err:
			exit(err)

	# Extract the heap begin and end addresses
	heap_begin, heap_end = [int(addr, 16) for addr in heap.split("-")]
	print(f"[~] HEAP BEGIN: [{heap_begin:#x}] | HEAP END: [{heap_end:#x}]")

	try:
			with open(f"/proc/{process_id}/mem", "r+b") as memory:
					# Seek to the beginning of the heap in the process memory
					memory.seek(heap_begin)
					# Search for the search string in the heap
					address = memory.read(heap_end - heap_begin).find(search_string) + heap_begin
					if address >= heap_begin:
							print(f"[~] \"{argv[2]}\" FOUND AT {address:#x}")
							memory.seek(address)
							memory.write(replace_string)

							# If the replace string is shorter, null terminate the remaining bytes
							if len(replace_string) < len(search_string):
									memory.seek(address + len(replace_string))
									memory.write(b'\0')
							print(f"[~] \"{argv[3]}\" WRITTEN AT {address:#x}")
							print("SUCCESS!")  # Print "SUCCESS!" upon successful replacement
					else:
							print(f"[~] \"{argv[2]}\" NOT FOUND")
							print("FAIL!")  # Print "FAIL!" if the search string is not found
	except FileNotFoundError as err:
			exit(err)

	# Exit with a success status code
	exit(0)

if __name__ == "__main__":
	main()
