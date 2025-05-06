#ifndef __U0DATE_H__
#define __U0DATE_H__

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define ESC 27
#define BACKSPACE 127
#define ctrl(x) ((x) & 0x1f)

// type definitions
typedef struct {
  char *contents;
  size_t size;      // Length of text in the line
  size_t capacity;  // Allocated memory capacity for the line (includes null terminator)
} row;

typedef struct {
  row *rows;
  size_t num_rows;
  size_t capacity;
} buffer;

typedef enum { NORMAL, INSERT } EditorMode;

/* functions */
const char *stringify_mode(EditorMode mode);

/* file functions */
int read_from_file(const char *filename, char *buffer, size_t buffer_s);
void save_to_file(const char *filename, char *buffer, size_t buffer_s);

/* row functions */
void init_row(row *r);
void free_row(row *r);
void ensure_row_capacity(row *r, size_t needed);

/* buffer functions */
void init_buffer(buffer *buf);
void free_buffer(buffer *buf);
void ensure_buffer_capacity(buffer *buf, size_t needed_capacity);

#endif
