#!/bin/bash
# bash run_imap_list.sh exe_path/imap list.txt 10m >>logfile 2>&1

files=`cat $2`
for element in ${files[@]}
do  
    echo $element
    if echo "$element" | grep -q -E '\.aig$'
    then
         timeout $3 $1 -c "read_aiger -f $element; print_stats; refactor; print_stats; rewrite; print_stats; balance; print_stats; refactor; print_stats; rewrite; print_stats;";
    else
        echo "$element not end with .txt"
    fi
done
