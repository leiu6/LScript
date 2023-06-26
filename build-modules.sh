#!/bin/bash

for source in ./modules/*.c
do
    echo "$source"
    gcc -shared -o ${source%.*}.so -fPIC $source
done