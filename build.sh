#!/bin/zsh
gcc *.c -lncurses -o out.o && ./out.o && rm *.o
