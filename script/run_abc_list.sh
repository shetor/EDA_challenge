#!/bin/bash

# run abc for each file in list
# bash run_abc_list.sh exe_path/abc list.txt 10m >>logfile2 2>&1

files=`cat $2`
for element in ${files[@]}
do  
    echo $element
    if echo "$element" | grep -q -E '\.aig$'
    then
         timeout $3 $1 -c "read_aiger $element; print_stats; if -K 6; print_stats";
    else
        echo "$element not end with .txt"
    fi
done
