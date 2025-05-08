#include "main.h"

void init_buffer(buffer *buf) {
  buf->num_rows = 0;
  buf->capacity = 1;
  buf->rows = malloc(buf->capacity * sizeof(row));
  if (buf->rows == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
}

void free_buffer(buffer *buf) {
  for (size_t i = 0; i < buf->num_rows; ++i) {
    free_row(&buf->rows[i]);
  }
  free(buf->rows);
  buf->rows = NULL;
  buf->num_rows = 0;
  buf->capacity = 0;
}

void display_buffer(const buffer *buf, EditorMode mode) {
  clear();
  size_t win_max_y, win_max_x __attribute__((unused));
  getmaxyx(stdscr, win_max_y, win_max_x);

  for (size_t i = 0; i < buf->num_rows && i < win_max_y - 1; ++i) {
    mvprintw(i, 0, buf->rows[i].contents);
    clrtoeol();
  }

  for (size_t i = buf->num_rows; i < win_max_y - 1; ++i) {
    mvprintw(i, 0, "");
    clrtoeol();
  }
  mvprintw(win_max_y - 2, 0, stringify_mode(mode));
  refresh();
}

void ensure_buffer_capacity(buffer *buf, size_t needed_capacity) {
  if (buf->capacity < needed_capacity) {
    size_t new_capacity = buf->capacity == 0 ? 2 : buf->capacity;
    while (new_capacity < needed_capacity) {
      new_capacity *= 2;
    }
    row *new_rows = realloc(buf->rows, new_capacity * sizeof(row));
    if (new_rows == NULL) {
      perror("realloc failed");
      exit(EXIT_FAILURE);
    }
    buf->rows = new_rows;
    buf->capacity = new_capacity;
  }
}