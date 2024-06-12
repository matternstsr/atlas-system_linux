#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks through the heap of a given process for string and replaces it with
another one.
"""

import sys
import os

def find_replace_string(pid, search_string, replace_string):
	"""
	Find and replace a string in the heap of a running process.
	Args:
		pid (str): The process ID of the running process.
		search_string (str): The string to search for in the heap.
		replace_string (str): The string to replace the found string with.
	Raises:
		FileNotFoundError: If the process does not exist.
		IOError: If there is an error reading or writing process memory.
	Returns:
		None
	"""

	# Check if there is a process
	if not os.path.exists(f"/proc/{pid}"):
		print("Error: Process not found.")
		sys.exit(1)
	try:
		# Read the heap memory map of the process
		with open(f"/proc/{pid}/maps", 'r') as maps_file:
			for line in maps_file:
				if "[heap]" in line:
					# Get the start and end addresses of the heap
					heap_start, heap_end = map(lambda x: int(x, 16),
                                line.split()[0].split('-'))
					break
	except Exception as e:
		print(f"Error reading memory map: {e}")
		sys.exit(1)

	try:
		# Open the process memory file
		with open(f"/proc/{pid}/mem", 'rb+', 0) as mem_file:
			# Move to the start of the heap
			mem_file.seek(heap_start)
			# Read the heap content
			heap_content = mem_file.read(heap_end - heap_start)
			# Find and replace the string
			if search_string.encode() in heap_content:
				heap_content = heap_content.replace(search_string.encode(),
                                        replace_string.encode())
				# Move back to the start of the heap and write the modified content
				mem_file.seek(heap_start)
				mem_file.write(heap_content)
				print("String replaced successfully.")
			else:
				print("Search string not found in the heap.")
	except Exception as e:
		print(f"Error accessing process memory: {e}")
		sys.exit(1)

if __name__ == "__main__":
	# Check for correct usage
	if len(sys.argv) != 4:
		print("Usage: read_write_heap.py pid search_string replace_string")
		sys.exit(1)
	pid = sys.argv[1]
	search_string = sys.argv[2]
	replace_string = sys.argv[3]
	# Call the function to find and replace the string
	find_replace_string(pid, search_string, replace_string)
