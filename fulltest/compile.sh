#!/bin/bash

DIRS=()

echo $SPLIT
echo "Creating missing directories"
echo $SPLIT

OPS=(${BASE_OPS[@]} ${EXTRA_OPS[@]})

mkdir -p "bin/"

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

p() { [ $# -eq 0 ] && echo || (shift; p "$@") |
    while read r; do
        echo $1 $r
        echo $r
    done
}


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

for o in $EXTRA_OPS
do
    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        echo gcc -O1 -$o ${DISABLE[@]} -o bin/$o/"$fname" "$f"
        gcc -O1 -$o ${DISABLE[@]} -o bin/$o/"$fname" "$f"
    done
done
