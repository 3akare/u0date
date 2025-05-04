#!/bin/bash

FILENAME="${1:-file.txt}"
gcc *.c -lncurses -o out.o && ./out.o $FILENAME && rm *.o
exit 1