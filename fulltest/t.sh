#!/bin/bash

LIST=(a b c d e f g)

p() { [ $# -eq 0 ] && echo || (shift; p "$@") |
    while read r; do
        echo $1 $r
        echo $r
    done
}

L=()
for ((i=0 ; i<${#LIST[@]} ; i++))
do
    L+=($i)
done

x=()

p ${L[@]} > "bin/perms"
while read line
do
    echo $line
done < "bin/perms"

