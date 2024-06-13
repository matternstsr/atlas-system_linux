#!/usr/bin/python3

"""
Python - Python - /proc filesystem - 0. Hack the VM

	Looks thru the heap of a given process for a str and rep with another one.
"""

import sys
import os

def print_usage_and_exit():
    print('Usage: {} pid search replace'.format(sys.argv[0]))
    sys.exit(1)

def is_ascii(s):
    return all(ord(c) < 128 for c in s)

if len(sys.argv) != 4:
    print_usage_and_exit()

pid = int(sys.argv[1])
if pid <= 0:
    print_usage_and_exit()

search_string = str(sys.argv[2])
if not search_string or not is_ascii(search_string):
    print("Error: Search string must be non-empty ASCII characters.")
    sys.exit(1)

replace_string = str(sys.argv[3])
if not replace_string or not is_ascii(replace_string):
    print("Error: Replace string must be non-empty ASCII characters.")
    sys.exit(1)

maps_filename = "/proc/{}/maps".format(pid)
mem_filename = "/proc/{}/mem".format(pid)

try:
    with open(maps_filename, 'r') as maps_file:
        for line in maps_file:
            sline = line.split(' ')
            if sline[-1][:-1] != "[heap]":
                continue

            addr = sline[0].split("-")
            if len(addr) != 2:
                print("[*] Wrong addr format")
                continue

            addr_start = int(addr[0], 16)
            addr_end = int(addr[1], 16)

            with open(mem_filename, 'rb+') as mem_file:
                mem_file.seek(addr_start)
                heap = mem_file.read(addr_end - addr_start)

                try:
                    i = heap.index(bytes(search_string, "ASCII"))
                except ValueError:
                    print("Can't find '{}'".format(search_string))
                    continue

                print("[*] Found '{}' at {:x}".format(search_string, i))
                if len(replace_string) > len(search_string):
                    print("Error: Replacement string is longer than search string.")
                    continue

                print("[*] Writing '{}' at {:x}".format(replace_string, addr_start + i))
                mem_file.seek(addr_start + i)
                mem_file.write(bytes(replace_string, "ASCII"))

                print("Replacement successful.")
                break

except IOError as e:
    print(f"Error: {e}")
    sys.exit(1)
