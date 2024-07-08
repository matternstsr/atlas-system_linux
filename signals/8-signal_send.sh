#!/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <pid>"
	exit 1
fi

# Get the PID from the first argument
pid=$1

# Check if PID is a positive number
if ! [[ "$pid" =~ ^[0-9]+$ ]]; then
	exit 1
fi

# Send SIGQUIT signal to the current process
kill -QUIT "$pid"

# Check if the kill command was successful
if [ $? -ne 0 ]; then
	exit 1
fi

exit 0
