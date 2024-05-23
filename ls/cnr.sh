#!/bin/bash

# Flag to track if memory leaks are detected
memory_leaks_detected=0

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
    memory_leaks_detected=0
    none_detected=0
    test1="./hls"
    test2="./hls test"
    test3="./hls test/folder1/ test"
    test4="./hls test/random"
    test5="./hls test -1"
    test6="./hls -a test"
    test7="./hls -a test/folder1/"
    test8="./hls -A test"
    test9="./hls -A test/folder1/"
    test10="./hls -l test"

    echo "Running Valgrind..."
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test1 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test1 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test1
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test2 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls test |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test2 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test2
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test/folder1/ test |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test3 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls test/folder1/ test |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test3 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test3
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test/random |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test4 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls test/random |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test4 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test4
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls test -1 |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test5 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls test -1 |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test5 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test5
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -a test |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY for $test6 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls -a test |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS for $test6 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test6
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -a test/folder1 |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY $test7 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls -a test/folder1 |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS $test7 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test7
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -A test |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY $test8 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls -A test |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS $test8 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test8
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -A test/folder1 |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY $test9 @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls -A test/folder1 |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@ NO MEMORY LEAKS $test9 @@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test9
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
    echo "                                                        "
    valgrind --leak-check=full ./hls -l test |& grep -q "definitely lost" && { echo -e "\e[31m@@@@@@@ LOST MEMORY $test10  @@@@@@@\e[0m"; memory_leaks_detected=1; none_detected=0; }
    valgrind --leak-check=full ./hls -l test |& grep -q "no leaks are possible" && { echo -e "\e[32m@@@@@@@@@@ NO MEMORY LEAKS $test10  @@@@@@@@\e[0m"; none_detected=1; memory_leaks_detected=0; }
    if [ $none_detected -eq 0 ]; then
    valgrind --leak-check=full $test10
    else none_detected=0
    fi
    echo "                                                        "
    echo "########################################################"
}

# Main menu function
clear
main_menu() {
echo -e "\e[34m Thank You for using Matt Checker 1.0 \e[0m"
    while true; do
        echo -e "\e[34mSelect an option:\e[0m"
        echo "1. Perform Betty style checks"
        echo "2. Perform Betty documentation checks"
        echo "3. Compile only"
        echo "4. Run Valgrind"
        echo -e "\e[31m5. Compile and Run Valgrind (Check for memory leaks)\e[0m"
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
                    if [ $memory_leaks_detected -eq 1 ]; then
                        echo -e "\e[31m@@@@@@@ YOU HAVE MEMORY LEAKS! @@@@@@@\e[0m"
                    else
                        echo -e "\e[32mAll good!\e[0m"
                    fi
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