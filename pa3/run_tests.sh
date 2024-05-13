#!/bin/bash

# Directory containing the files
directory="good_tests"

# Executable paths
executable1="./mysemant "
executable2="./lexer "

echo -e "\e[32m \t\t\t RUNNING GOOD TESTS \t\t\t \e[0m"
# Loop through each file in the directory

for file in $directory/*; do
    # Run both executables on the file and capture their outputs
    echo "$file"
    # output1="temp1.txt"
    # output2="temp2.txt"

    # $executable1 < "$file" > "$output1"
    # cat "$file" | $executable2 | ./parser | /afs/ir/class/cs143/bin/semant > "$output2"

    # echo "Output of $executable1 for $(basename "$file"):"
    # cat "$output1"
    # echo "Output of $executable2 | ./parser | /afs/ir/class/cs143/bin/semant for $(basename "$file"):"
    # cat "$output2"

    # Compare the sorted outputs
    # diff <(./lexer "$file" | ./parser | /afs/ir/class/cs143/bin/semant | sort) <(./mysemant "$file" | sort)
    if diff <(./lexer "$file" | ./parser | /afs/ir/class/cs143/bin/semant | sort) <(./mysemant "$file" | sort) > /dev/null 2>&1; then
        echo -e "Comparing output on $(basename "$file"): \e[32mCorrect\e[0m"
    else
        diff <(./lexer "$file" | ./parser | /afs/ir/class/cs143/bin/semant | sort) <(./mysemant "$file" | sort)
        echo -e "Comparing output on $(basename "$file"): \e[31mIncorrect\e[0m"
    fi

    #diff <(./lexer "$file" | ./parser | /afs/ir/class/cs143/bin/semant 2>&1 | sort) <(./mysemant "$file" 2>&1 | sort)  > /dev/null 2>&1


    # Clean up temporary files
    # rm "$output1" "$output2"
    # diff_result=$(diff <(./lexer "$file" | ./parser | /afs/ir/class/cs143/bin/semant | sort) <(./mysemant "$file" | sort) > /dev/null 2>&1; echo $?)
    # if [ "$diff_result" -eq 0 ]; then
    #     echo -e "Comparing output on $(basename "$file"): \e[32mCorrect\e[0m"
    # else
    #     echo -e "Comparing output on $(basename "$file"): \e[31mIncorrect\e[0m"
    # fi
done

# echo -e "\e[31m \t\t\t RUNNING BAD TESTS \t\t\t \e[0m"

# bad_directory="bad_tests"
# for file in $directory/*; do
#     # Run both executables on the file and capture their outputs
#     output1="temp1.txt"
#     output2="temp2.txt"

#     $executable1 < "$file" > "$output1"
#     cat "$file" | $executable2 | ./parser | /afs/ir/class/cs143/bin/semant > "$output2"

#     echo "Output of $executable1 for $(basename "$file"):"
#     cat "$output1"
#     echo "Output of $executable2 | ./parser | /afs/ir/class/cs143/bin/semant for $(basename "$file"):"
#     cat "$output2"

#     # Compare the sorted outputs
#     if diff <(sort "$output1") <(sort "$output2") > /dev/null; then
#         echo -e "Comparing output on $(basename "$file"): \e[32mCorrect\e[0m"
#     else
#         echo -e "Comparing output on $(basename "$file"): \e[31mIncorrect\e[0m"
#         echo diff <(sort "$output1") <(sort "$output2")
#     fi

#     # Clean up temporary files
#     rm "$output1" "$output2"
# done
