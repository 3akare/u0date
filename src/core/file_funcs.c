#include "main.h"

void read_file_into_buffer(const char *filename, buffer *buf) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    insert_row(buf, 0);
    return;
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  while ((linelen = getline(&line, &linecap, f)) != -1) {
    if (linelen > 0 && line[linelen - 1] == '\n') {
      linelen--;
      line[linelen] = '\0';
    }
    insert_row(buf, buf->num_rows);
    row *new_row = &buf->rows[buf->num_rows - 1];
    ensure_row_capacity(new_row, linelen);
    memcpy(new_row->contents, line, linelen + 1);
    new_row->size = linelen;
  }

  if (buf->num_rows == 0) {
    insert_row(buf, 0);
  }
  free(line);
  fclose(f);
}

void save_file_from_buffer(const char *filename, const buffer *buf) {
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    perror("Failed to save files");
    return;
  }

  for (size_t i = 0; i < buf->num_rows; ++i) {
    fwrite(buf->rows[i].contents, 1, buf->rows[i].size, f);
    if (i < buf->num_rows - 1) {
      fwrite("\n", 1, 1, f);
    }
  }
  fclose(f);
}