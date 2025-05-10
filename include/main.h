#ifndef __U0DATE_H__
#define __U0DATE_H__

#include <errno.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define ESC 27
#define BACKSPACE 127
#define CTRL(x) ((x) & 0x1f)
#define SAVE_KEY CTRL('s')
#define UNDO_KEY 'u'
#define REDO_KEY CTRL('r')

#define min(a, b) ((a) < (b) ? (a) : (b))

typedef enum { NORMAL, INSERT } EditorMode;
typedef enum {
  ACTION_INSERT_CHAR,
  ACTION_DELETE_CHAR,
  ACTION_INSERT_ROW,
  ACTION_DELETE_ROW,
  ACTION_SPLIT_ROW,
  ACTION_JOIN_ROWS
} ActionType;

typedef struct {
  char *contents;
  size_t size;      // Length of text in the line
  size_t capacity;  // Allocated memory capacity for the line (includes null
                    // terminator)
} row;

typedef struct {
  row *rows;
  size_t num_rows;
  size_t capacity;
} buffer;

typedef struct {
  ActionType type;
  size_t row_index;
  size_t col_index;
  char *data;
  size_t data_size;
  size_t original_row1_size;
} action;

typedef struct {
  action *actions;
  size_t count;
  size_t capacity;
} history;

/* functions */
const char *stringify_mode(EditorMode mode);

/* file functions */
void read_file_into_buffer(const char *filename, buffer *buf);
void save_file_from_buffer(const char *filename, const buffer *buf);

/* row functions */
void init_row(row *r);
void free_row(row *r);
void insert_row(buffer *buf, size_t index);
void delete_row(buffer *buf, size_t index);
void insert_char_in_row(buffer *buf, size_t row_index, size_t col_index,
                        int ch);
void delete_char_in_row(buffer *buf, size_t row_index, size_t col_index);
void split_row(buffer *buf, size_t row_index, size_t col_index);
void join_rows(buffer *buf, size_t row1_index, size_t row2_index);
void ensure_row_capacity(row *r, size_t needed);

/* buffer functions */
void init_buffer(buffer *buf);
void free_buffer(buffer *buf);
void display_buffer(const buffer *buf, EditorMode mode);
void ensure_buffer_capacity(buffer *buf, size_t needed_capacity);

/* action functions */
void init_action(action *a);
void free_action(action *a);
void push_action(history *h, action *a);
int pop_action(history *h, action *out_a);

/* history functions */
void init_history(history *h);
void free_history(history *h);
void clear_history(history *h);
void ensure_history_capacity(history *h, size_t needed_capacity);

#endif
