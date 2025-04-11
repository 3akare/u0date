#ifndef __U0DATE_H__
#define __U0DATE_H__


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ncurses.h>

#define ESC 27
#define BACKSPACE 127
#define ctrl(x) ((x) & 0x1f)

typedef enum{
    NORMAL,
    INSERT
} Mode;

/* functions */
const char* stringify_mode(Mode mode);

#endif
