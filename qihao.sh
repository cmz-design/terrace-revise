#!/bin/bash


for file in `ls ~/data/real_data/*txt`
do
    ./test_powlaw $file 100000
done
