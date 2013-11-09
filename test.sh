#!/bin/bash

ITERS=10

echo $SPLIT
echo "Timing runs"
echo $SPLIT
for d in $DIRS
do
    FFILES=(`ls $d`)
    for f in $FFILES
    do
        echo -e "$d/$f"
        tot=0
        for ((i=1; i<=$ITERS; i++))
        #for i in $(eval echo "{1..$ITERS}")
        do
            echo -ne " $i"
            #time prints to stderr, have to pipe to stdout
            #unfortunately this breaks syntax highlighting
            rs=$((/usr/bin/time -f %U ./$d/$f > /dev/null) 2>&1)
            tot=$(echo $tot + $rs | bc)
        done
        tot=$(echo $tot / $ITERS | bc -l)
        echo $tot >> "$d/$f"result.txt
        echo -e "\n"
    done
done
