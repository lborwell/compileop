#!/bin/bash

ITERS=1
MIN_TIME=2

echo $SPLIT
echo "Timing runs"
echo $SPLIT

FILES=bin/*

echo -ne "test name," >> results.csv
for d in $DIRS
do
    echo -ne "$d," >> results.csv
done

for f in ${FILES[@]}
do
    echo -ne "\n$f," >> results.csv
    for d in $DIRS
    do
        echo -e "\n$d/$f"
        otot=0
        itot=0
        for ((i=1; i<=$ITERS; i++))
        do
            counter=0
            while [ 0 == 0 ]
            do
                counter=`expr $counter + 1`
                echo -ne " $i"
                #time prints to stderr, have to pipe to stdout
                #unfortunately this breaks syntax highlighting
                rs=$((/usr/bin/time -f %U ./$d/$f > /dev/null) 2>&1) #))
                itot=$(echo $itot + $rs | bc)
                if [ $itot > $MIN_TIME ] 
                then
                    itot=$(echo "$itot/$counter" | bc -l)
                    otot=$(echo $otot + $itot | bc -l)
                    break;
                fi
            done
        done
        tot=$(echo $otot / $ITERS | bc -l)
        echo -ne "$otot," >> results.csv
    done
    echo -e "\n"
done
