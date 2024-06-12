#!/usr/bin/python3

"""
Finds and substitutes the initial instance of a string within the heap
memory of a process.

Usage: ./read_write_heap.py PID search_string replace_by_string
Where:
- PID refers to the process ID of the target process.
- search_string denotes the ASCII string you wish to replace.
- replace_by_string represents the ASCII string you want to use
  as a replacement for the search_string.
"""

import sys
import os

def print_usage_and_exit():
	print('Usage: {} pid search write'.format(sys.argv[0]))
	sys.exit(1)

def open_files(pid):
	maps_filename = "/proc/{}/maps".format(pid)
	print("[*] maps: {}".format(maps_filename))
	mem_filename = "/proc/{}/mem".format(pid)
	print("[*] mem: {}".format(mem_filename))
	try:
		maps_file = open(maps_filename, 'r')
		return maps_file, mem_filename
	except IOError as e:
		print("[ERROR] Can not open file {}:".format(maps_filename))
		print("        I/O error({}): {}".format(e.errno, e.strerror))
		sys.exit(1)

def find_heap_address(maps_file):
	for line in maps_file:
		sline = line.split(' ')
		if sline[-1][:-1] == "[heap]":
			return sline[0].split("-")
	print("[*] Heap not found.")
	maps_file.close()
	sys.exit(1)

def parse_heap_info(heap_info):
	if len(heap_info) != 2:
		print("[*] Wrong addr format")
		sys.exit(1)
	return int(heap_info[0], 16), int(heap_info[1], 16)

def main():
	if len(sys.argv) != 4:
		print_usage_and_exit()
	pid = int(sys.argv[1])
	if pid <= 0:
		print_usage_and_exit()
	search_string = str(sys.argv[2])
	if search_string == "":
		print_usage_and_exit()
	write_string = str(sys.argv[3])
	if write_string == "":
		print_usage_and_exit()
	maps_file, mem_filename = open_files(pid)
	addr_start, addr_end = parse_heap_info(find_heap_address(maps_file))
	try:
		with open(mem_filename, 'rb+') as mem_file:
			mem_file.seek(addr_start)
			heap = mem_file.read(addr_end - addr_start)
		try:
			i = heap.index(bytes(search_string, "ASCII"))
		except ValueError:
			print("Can't find '{}'".format(search_string))
			sys.exit(0)
		print("[*] Found '{}' at {:x}".format(search_string, i))
		print("[*] Writing '{}' at {:x}".format(write_string, addr_start + i))
		mem_file.seek(addr_start + i)
		mem_file.write(bytes(write_string, "ASCII"))
	except IOError as e:
		print("[ERROR] Can not open file {}:".format(mem_filename))
		print("        I/O error({}): {}".format(e.errno, e.strerror))
		sys.exit(1)

if __name__ == "__main__":
	main()