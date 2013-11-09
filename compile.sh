#!/bin/bash

OPS="O0
O1
O2
O3
funroll-loops
fno-unroll-loops"

DIRS=()

echo $SPLIT
echo "Creating missing directories"
echo $SPLIT
for o in $OPS
do
    d=(" bin/$o")
    DIRS+=$d
    if [ ! -d $d ]
    then
        echo mkdir -p $d
    fi
    mkdir -p $d
done


echo $SPLIT
echo "Compiling files"
echo $SPLIT
FILES=src/*.c
FNAMES=()

for f in $FILES
do
    fname=$(basename "$f")
    fname="${fname%.*}"
    FNAMES+=("$fname")
done

for o in $OPS
do
    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        echo gcc -$o -o bin/$o/"$fname" "$f"
        gcc -$o -o bin/$o/"$fname" "$f"
    done
done

