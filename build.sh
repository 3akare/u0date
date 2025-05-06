#!/bin/bash

FILENAME="${1:-file.txt}"

gcc -Iinclude $(find src -name "*.c") -lncurses -o build/u0date ;
./build/u0date $FILENAME

cat $FILENAME
if [ $2 ]; then
    rm $FILENAME
fi

exit 1