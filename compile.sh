#!/bin/bash

DIRS="bin/o0
bin/o1
bin/o2
bin/o3
bin/lunroll
bin/lnoroll"

SPLIT="============================"

echo $SPLIT 
echo "Creating missing directories"
for d in $DIRS
do
    if [ ! -d $d ] 
      then
        echo mkdir -p $d
    fi
    mkdir -p $d
done

echo $SPLIT
echo "Compiling files"
FILES=src/*.c
for f in $FILES
do
    fname=$(basename "$f")
    fname="${fname%.*}"
    gcc -O0 -o bin/o0/"$fname" "$f"
    gcc -O1 -o bin/o1/"$fname" "$f"
    gcc -O2 -o bin/o2/"$fname" "$f"
    gcc -O3 -o bin/o3/"$fname" "$f"
    gcc -O3 -funroll-loops -o bin/lunroll/"$fname" "$f"
    gcc -O3 -fno-unroll-loops -o bin/lnoroll/"$fname" "$f"
done

