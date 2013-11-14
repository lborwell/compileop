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

L=()
for ((i=0; i<${#OPS[@]} ; i++))
do
    L+=($i)
done

p ${L[@]} > "bin/perms"

while read line
do
    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        file=$fname
        opl=()
        for i in ${line[@]}
        do
            file+=$i
            opl+=(-${OPS[$i]})
        done
        gcc ${opl[@]} -o "bin/$file" "$f"
        echo gcc ${opl[@]} -o "bin/$file" "$f"
    done
done < "bin/perms"
