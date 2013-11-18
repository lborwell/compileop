#!/bin/bash

source lists.sh
source clear.sh
if [ $# -eq 0 ]; then
    source compile.sh "A"
    source test.sh
    exit
fi

source compile.sh "S"
