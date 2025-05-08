#include "main.h"

void init_row(row *r) {
  r->size = 0;
  r->capacity = 1;
  r->contents = malloc(r->capacity);
  if (r->contents == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  r->contents[0] = '\0';
}

void free_row(row *r) {
  free(r->contents);
  r->contents = NULL;
  r->size = 0;
  r->capacity = 0;
}

void insert_row(buffer *buf, size_t index) {
  if (index > buf->num_rows) return;
  ensure_buffer_capacity(buf, buf->num_rows + 1);
  memmove(&buf->rows[index + 1], &buf->rows[index],
          (buf->num_rows - index) * sizeof(row));
  init_row(&buf->rows[index]);
  buf->num_rows++;
}

void delete_row(buffer *buf, size_t index) {
  if (index >= buf->num_rows) return;
  free_row(&buf->rows[index]);
  memmove(&buf->rows[index], &buf->rows[index + 1],
          (buf->num_rows - index - 1) * sizeof(row));
  buf->num_rows--;
}

void insert_char_in_row(buffer *buf, size_t row_index, size_t col_index,
                        int ch) {
  if (row_index >= buf->num_rows) return;
  row *r = &buf->rows[row_index];
  if (col_index > r->size) col_index = r->size;
  ensure_row_capacity(r, r->size + 1);
  memmove(&r->contents[col_index + 1], &r->contents[col_index],
          (r->size - col_index + 1));
  r->contents[col_index] = ch;
  r->size++;
}

void delete_char_in_row(buffer *buf, size_t row_index, size_t col_index) {
  if (row_index >= buf->num_rows) return;
  row *r = &buf->rows[row_index];
  if (col_index >= r->size) return;
  memmove(&r->contents[col_index], &r->contents[col_index + 1],
          (r->size - col_index));
  r->size--;
  r->contents[r->size] = '\0';
}

void split_row(buffer *buf, size_t row_index, size_t col_index) {
  if (row_index >= buf->num_rows) return;
  row *r = &buf->rows[row_index];
  if (col_index > r->size) col_index = r->size;

  insert_row(buf, row_index + 1);
  row *new_r = &buf->rows[row_index + 1];

  size_t chars_to_move = r->size - col_index;
  if (chars_to_move > 0) {
    ensure_row_capacity(new_r, chars_to_move);
    memcpy(new_r->contents, &r->contents[col_index], chars_to_move);
    new_r->size = chars_to_move;
    new_r->contents[new_r->size] = '\0';
  }
  r->size = col_index;
  r->contents[r->size] = '\0';
}

void join_rows(buffer *buf, size_t row1_index, size_t row2_index) {
  if (row1_index >= buf->num_rows - 1 || row2_index != row1_index + 1) return;
  row *r1 = &buf->rows[row1_index];
  row *r2 = &buf->rows[row2_index];

  size_t original_r1_size = r1->size;
  size_t char_to_join = r2->size;

  ensure_row_capacity(r1, r1->size + char_to_join);
  memcpy(&r1->contents[original_r1_size], r2->contents, char_to_join + 1);
  r1->size += char_to_join;
  delete_row(buf, row2_index);
}

void ensure_row_capacity(row *r, size_t needed) {
  size_t required_capacity = needed + 1;
  if (r->capacity < required_capacity) {
    size_t new_capacity = r->capacity == 0 ? 16 : r->capacity;
    while (new_capacity < required_capacity) {
      new_capacity *= 2;
    }
    char *new_contents = realloc(r->contents, new_capacity);
    if (new_contents == NULL) {
      perror("realloc failed");
      exit(EXIT_FAILURE);
    }
    r->contents = new_contents;
    r->capacity = new_capacity;
  }
}