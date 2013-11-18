#!/bin/bash

DIRS=()

echo $SPLIT
echo "Creating missing directories"
echo $SPLIT

LISTS=(${BASE_OPS[@]} ${EXTRA_OPS[@]})

for o in ${LISTS[@]}
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

for o in $BASE_OPS
do
    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        if [ ! $1 == "S" ]; then
            echo gcc -$o -o bin/$o/"$fname" "$f"
            gcc -$o -o bin/$o/"$fname" "$f"
        fi
        if [ $1 == "S" ]; then
            echo gcc -S -$o -o bin/$o/"$fname" "$f"
            gcc -S -$o -o bin/$o/"$fname" "$f"
        fi
    done
done


for o in $EXTRA_OPS
do
    dis=()
    for d in ${DISABLE[@]}
    do
        add="true"
        if [ ! $o == $d ]; then
            dis+=" -fno-$d"
        fi
    done
    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        if [ ! $1 == "S" ]; then
            echo gcc -O1 -f$o ${dis[@]} -o bin/$o/"$fname" "$f"
            gcc -O1 "-f$o" ${dis[@]} -o bin/$o/"$fname" "$f"
        fi
        if [ $1 == "S" ]; then
            echo gcc -S -O1 -f$o ${dis[@]} -o bin/$o/"$fname" "$f"
            gcc -S -O1 "-f$o" ${dis[@]} -o bin/$o/"$fname" "$f"
        fi
    done
done
