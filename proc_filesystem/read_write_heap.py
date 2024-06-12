#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks through the heap of a given process for string and replaces it with
another one.
"""

import sys
import os

def find_replace_string(pid, search_string, replace_string):
	try:
		pid = int(pid)
	except ValueError:
		print("Invalid process ID")
		return

	if not os.path.exists(f"/proc/{pid}"):
		print("Process not found.")
		return

	try:
		with open(f"/proc/{pid}/maps", 'r') as maps_file:
			for line in maps_file:
				if "[heap]" in line:
					heap_start, heap_end = map(lambda x: int(x, 16), line.split()[0].split('-'))
					break
	except Exception as e:
		print(f"Error reading memory map: {e}")
		return

	try:
		with open(f"/proc/{pid}/mem", 'rb+') as mem_file:
			mem_file.seek(heap_start)
			heap_content = mem_file.read(heap_end - heap_start)
			search_bytes = search_string.encode()
			replace_bytes = replace_string.encode()
			if search_bytes in heap_content:
				heap_content = heap_content.replace(search_bytes, replace_bytes)
				mem_file.seek(heap_start)
				mem_file.write(heap_content)
				print("String replaced successfully.")
			else:
				print("Search string not found in the heap.")
	except Exception as e:
		print(f"Error accessing process memory: {e}")

if __name__ == "__main__":
	if len(sys.argv) != 4:
		print("Usage: python script.py <pid> <search_string> <replace_string>")
		sys.exit(1)
    
	pid = sys.argv[1]
	search_string = sys.argv[2]
	replace_string = sys.argv[3]
	
	find_replace_string(pid, search_string, replace_string)