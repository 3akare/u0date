#include "main.h"

int read_from_file(const char *filename, char *buffer, size_t buffer_s) {
  int ch = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) return 0;
  while ((ch = fgetc(file)) != EOF) {
    buffer[buffer_s++] = ch;
    addch(ch);
  }
  fclose(file);
  return buffer_s;
}

int save_to_file(const char *filename, char *buffer, size_t buffer_s) {
  FILE *file = fopen(filename, "wb");
  fwrite(buffer, 1, buffer_s, file);
  fclose(file);
}
