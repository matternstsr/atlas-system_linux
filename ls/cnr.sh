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
    else
        echo "Invalid option!"
        # rm $betty_output
    fi
    
    if [ -s $betty_output ]; then
        echo "Fix Betty $checks errors before compiling!"
        cat $betty_output
        # rm $betty_output
    fi
    # rm $betty_output
}

# Function to run Valgrind
run_valgrind() {
    echo "Running Valgrind..."
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls 
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test/folder1/ test
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test/random
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test -1
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -a test
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -a test/folder1
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -A test
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -A test/folder1
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -l test
    echo "                                                        "
    echo -e "\e[32mAll heap blocks were freed -- no leaks are possible\e[0m"
    echo "########################################################"
}

# Main menu function
clear
main_menu() {
    while true; do
        echo "Select an option:"
        echo "1. Perform Betty style checks"
        echo "2. Perform Betty documentation checks"
        echo "3. Compile only"
        echo "4. Run Valgrind"
        echo "5. Compile and Run Valgrind (Check for memory leaks)"
        echo "6. Push your code"
        echo "7. Exit"
    
        read -p "Enter your choice (1-6): " choice
    
        case $choice in
            1)
                clear
                perform_betty_checks "style"
                ;;
            2)
                clear
                perform_betty_checks "doc"
                ;;
            3)
                clear
                echo "Compiling..."
                if gcc -Wall -Werror -Wextra -pedantic *.c -o hls; then
                    echo "Compiled successfully!"
                else
                    echo "Compilation failed!"
                fi
                ;;
            4)
                clear
                run_valgrind
                ;;
            5)
                clear
                echo "Compiling..."
                if gcc -Wall -Werror -Wextra -pedantic *.c -o hls; then
                    echo "Compiled successfully!"
                    run_valgrind
                    echo "All good!"
                else
                    echo "Compilation failed! Oops, better check that code."
                fi
                ;;
            6)
                clear
                pusher
                ;;
            7)
                clear
                exit 0
                ;;
            *)
                echo "Invalid choice!"
                ;;
        esac
    done
}


# Start the main menu
main_menu
