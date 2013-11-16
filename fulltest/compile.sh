#!/bin/bash

DIRS=()
ITERS=20
MIN_TIME=2

echo $SPLIT
echo "Creating missing directories"
echo $SPLIT

OPS=(${EXTRA_OPS[@]})

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

echo -ne "test," >> "results.csv"
for f in $FILES
do
    echo -ne "$f," >> "results.csv"
done

while read line
do
    #print row title
    echo -ne "\n" >> results.csv
    for i in ${line[@]}; do
        echo -ne $i >> results.csv
    done

    for f in $FILES
    do
        fname=$(basename "$f")
        fname="${fname%.*}"

        file=$fname
        opl=()
        for i in ${line[@]}
        do
            file+=$i
            opl+=" -f${OPS[$i]}"
        done
        
        dis=()
        for d in ${DISABLE[@]}
        do
            add="true"
            for o in ${OPS[@]}
            do
                if [ $o == $d ]; then
                    add="false"
                fi
            done
            if [ $add == "true" ]; then
                dis+=" -fno-$d"
            fi
        done

        gcc ${dis[@]} ${opl[@]} -O -o "bin/$file" "$f"
        echo gcc ${dis[@]} ${opl[@]} -O -o "bin/$file" "$f"

        #timing

        otot=0
        itot=0
        echo -e "\n$file"
        for ((i=1; i<=$ITERS; i++))
        do
            counter=0
            while [ 0 == 0 ]
            do
                counter=`expr $counter + 1`
                echo -ne " $i"
                #time prints to stderr, have to pipe to stdout
                #unfortunately this breaks syntax highlighting
                rs=$((/usr/bin/time -f %U ./bin/$file > /dev/null) 2>&1) #))
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
        echo -ne ",$tot" >> results.csv
        echo -e "\n"

        rm "bin/$file"
    done
done < "bin/perms"
