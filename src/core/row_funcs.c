#include "main.h"

void init_row(row *r) {
  r->size = 0;
  r->capacity = 1;
  r->contents = malloc(r->capacity);
  if (!r->contents) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
}

void free_row(row *r) {
  free(r->contents);
  r->contents = NULL;
  r->size = 0;
  r->capacity = 0;
}

void ensure_row_capacity(row *r, size_t needed) {
  size_t required_capacity = needed + 1;
  if (r->capacity < required_capacity) {
    size_t new_capacity = r->capacity == 0 ? 16 : r->capacity;
    while (new_capacity < required_capacity) {
      new_capacity *= 2;
    }
    char *new_contents = realloc(r->contents, new_capacity);
    if (!new_contents) {
      perror("realloc failed");
      exit(EXIT_FAILURE);
    }
    r->capacity = new_capacity;
    r->contents = new_contents;
  }
}