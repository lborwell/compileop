#!/bin/bash

DIRS="bin/o0
bin/o1
bin/o2
bin/o3
bin/lunroll
bin/lnoroll"

echo $SPLIT
echo "Timing runs"
for d in $DIRS
do
    FFILES=(`ls $d`)
    for f in $FFILES
    do
        tot=0
        for i in {1..10}
        do
            echo -e "$d/$f $i"
            #time prints to stderr, have to pipe to stdout
            rs=$((/usr/bin/time -f %U ./$d/$f > /dev/null) 2>&1)
            tot=$(echo $tot + $rs | bc)
        done
        tot=$(echo $tot / 10 | bc -l)
        echo $tot >> "$d/$f"result.txt
        echo -e "\n"
    done
    echo -e "\n"
done
