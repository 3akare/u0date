#include "main.h"

void init_buffer(buffer *buf) {
	buf->num_rows = 0;
	buf->capacity = 1;
	buf->rows = malloc(buf->capacity * sizeof(row));
	if (!buf->rows) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
}

void free_buffer(buffer *buf) {
	for (size_t i = 0; i < buf->num_rows; ++i) {
		free(&buf->rows[i]);
	}
	free(buf->rows);
	buf->rows = NULL;
	buf->capacity = 0;
	buf->num_rows = 0;
}

void ensure_buffer_capacity(buffer *buf, size_t needed_capacity) {
	size_t required_capacity = needed_capacity + 1;
	if (buf->capacity < required_capacity) {
		size_t new_capacity = buf->capacity == 0 ? 2 : buf->capacity;
		while (new_capacity < required_capacity) {
			new_capacity *= 2;
		}
		row *new_rows = realloc(buf->rows, new_capacity * sizeof(row));
		if (!new_rows) {
			perror("realloc failed");
			exit(EXIT_FAILURE);
		}
		buf->rows = new_rows;
		buf->capacity = new_capacity;
	}
}