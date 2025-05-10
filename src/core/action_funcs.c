#include "main.h"

void init_action(action *a) {
  a->type = ACTION_INSERT_CHAR;
  a->row_index = 0;
  a->col_index = 0;
  a->data = NULL;
  a->data_size = 0;
  a->original_row1_size = 0;
}

void free_action(action *a) {
  if (a->data) {
    free(a->data);
    a->data = NULL;
  }
  a->data_size = 0;
}

void push_action(history *h, action *a) {
  ensure_history_capacity(h, h->count + 1);
  h->actions[h->count] = *a;
  if (a->data_size > 0 && a->data != NULL) {
    h->actions[h->count].data = malloc(a->data_size + 1);
    if (h->actions[h->count].data == NULL) {
      perror("malloc action data failed");
    }
    memcpy(h->actions[h->count].data, a->data, a->data_size);
    h->actions[h->count].data[a->data_size] = '\0';
    h->actions[h->count].data_size = a->data_size;
  } else {
    h->actions[h->count].data = NULL;
    h->actions[h->count].data_size = 0;
  }
  h->count++;
}

int pop_action(history *h, action *out_a){
    if (h->count == 0) return 0;
    h->count--;
    *out_a = h->actions[h->count];
    return 1;
}