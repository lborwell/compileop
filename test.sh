#!/bin/bash

ITERS=1

echo $SPLIT
echo "Timing runs"
echo $SPLIT

echo -ne "test name," >> results.csv
for d in $DIRS
do
    echo -ne "$d," >> results.csv
done

for f in ${FNAMES[@]}
do
    echo -ne "\n$f," >> results.csv
    for d in $DIRS
    do
        echo -e "\n$d/$f"
        tot=0
        for ((i=1; i<=$ITERS; i++))
        do
            echo -ne " $i"
            #time prints to stderr, have to pipe to stdout
            #unfortunately this breaks syntax highlighting
            rs=$((/usr/bin/time -f %U ./$d/$f > /dev/null) 2>&1)
            tot=$(echo $tot + $rs | bc)
        done
        tot=$(echo $tot / $ITERS | bc -l)
        echo -ne "$tot," >> results.csv
        echo $tot >> "$d/$f"result.txt
    done
    echo -e "\n"
done
