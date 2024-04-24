#!/bin/bash
# Iterate over each .cl file in the tests directory.
# By default, no output after the file name is good news and means
# there is no difference from the reference solution. Other diff
# options may have different outputs (check the diff man page).
for file in tests/*.cl; do
    echo "$file"
    diff --color=auto "$@" <(/afs/ir/class/cs143/bin/lexer "$file" | /afs/ir/class/cs143/bin/parser) <(./myparser "$file")
done