#!/bin/bash

FILENAME="${1:-file.txt}"

gcc src/*.c -lncurses -o out.o && ./out.o $FILENAME && rm *.o

cat $FILENAME
if [ $2 ]; then
    rm $FILENAME
fi

exit 1