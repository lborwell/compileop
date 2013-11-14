#!/bin/bash

BASE_OPS="O0
O1
O2
O3"

EXTRA_OPS="funroll-loops
fno-unroll-loops"

DISABLE="-fno-auto-inc-dec 
-fno-cprop-registers 
-fno-dce 
-fno-defer-pop 
-fno-delayed-branch 
-fno-dse 
-fno-guess-branch-probability 
-fno-if-conversion2 
-fno-if-conversion 
-fno-ipa-pure-const 
-fno-ipa-reference 
-fno-merge-constants
-fno-split-wide-types 
-fno-tree-builtin-call-dce 
-fno-tree-ccp 
-fno-tree-ch 
-fno-tree-copyrename 
-fno-tree-dce 
-fno-tree-dominator-opts 
-fno-tree-dse 
-fno-tree-fre 
-fno-tree-sra 
-fno-tree-ter 
-fno-unit-at-a-time"