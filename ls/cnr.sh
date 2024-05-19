#!/bin/bash

# Function to perform Betty style checks
perform_betty_checks() {
    local checks=$1
    local betty_output=$(mktemp)
    
    echo "Running Betty $checks checks..."
    if [ "$checks" == "style" ]; then
        betty-style *.c > $betty_output
    elif [ "$checks" == "doc" ]; then
        betty-doc *.c > $betty_output
    elif [ "$checks" == "all" ]; then
        betty-style *.c > $betty_output
        betty-doc *.c >> $betty_output
    else
        echo "Invalid option!"
        rm $betty_output
        exit 1
    fi
    
    if [ -s $betty_output ]; then
        echo "Fix Betty $checks errors before compiling!"
        cat $betty_output
        # Run gcc with flags
        echo "Compiling..."
        gcc -Wall -Werror -Wextra -pedantic *.c -o hls
        # Run valgrinds
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls test
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls test/folder1/ test
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls test/random
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls test -1
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls -a test
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls -a test/folder1
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls -A test
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls -A test/folder1
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        valgrind ./hls -l test
        echo "                                                        "
        echo "########################################################"
        echo "                                                        "
        rm $betty_output
        exit 1
    fi
    
    rm $betty_output
}

# Ask user which Betty checks to perform
clear
read -p "Which Betty checks do you want to perform? (style/doc/all): " checks

# Perform Betty checks based on user input
case "$checks" in
    style)
        perform_betty_checks "style"
        ;;
    doc)
        perform_betty_checks "doc"
        ;;
    all)
        perform_betty_checks "all"
        ;;
    exit)
        exit 1
        ;;
    *)
        echo "Invalid option!"
        ./cnr.sh
        ;;
esac

# Run gcc with flags
echo "Compiling..."
gcc -Wall -Werror -Wextra -pedantic *.c -o hls

# Check if gcc had errors
if [ $? -eq 0 ]; then
    # If no errors, pause for 2 seconds
    sleep 2
    echo "Running valgrind..."
    # Run valgrind
    valgrind ./hls
else
    # If gcc had errors, display message
    echo "Fix it!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    # Run valgrind
    valgrind ./hls
    # Pause for 2 seconds
    sleep 2
fi
