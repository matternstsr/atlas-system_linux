#!/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <pid>"
	exit 1
fi

# Get the PID from the first argument
pid=$1
# Send SIGQUIT signal to the current process
kill -QUIT "$pid"
