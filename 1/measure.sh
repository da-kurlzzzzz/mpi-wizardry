#!/bin/bash

for NP in {1..28}
do
    for test in {1..2}
    do
        make NP=$NP run
    done 2>&1 >/dev/null | awk '{sum+=$1} END{print sum/NR}'
done | awk 'NR==1{base=$1} {print base/$1}' | nl >data.txt
