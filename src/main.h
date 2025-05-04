#ifndef __U0DATE_H__
#define __U0DATE_H__

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define ESC 27
#define BACKSPACE 127
#define ctrl(x) ((x) & 0x1f)

typedef enum { NORMAL, INSERT } Mode;

/* functions */
const char* stringify_mode(Mode mode);

/* file functions */
int read_from_file(const char *filename, char *buffer, size_t buffer_s);
int save_to_file(const char *filename, char *buffer, size_t buffer_s);

#endif
