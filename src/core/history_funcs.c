#include "main.h"

void init_history(history *h) {
  h->count = 0;
  h->capacity = 8;
  h->actions = malloc(h->capacity * sizeof(action));
  if (h->actions == NULL) {
    perror("malloc history failed");
    exit(EXIT_FAILURE);
  }
}

void free_history(history *h) {
  for (size_t i = 0; i < h->count; ++i) {
    free_action(&h->actions[i]);
  }
  free(h->actions);
  h->actions = NULL;
  h->count = 0;
  h->capacity = 0;
}

void ensure_history_capacity(history *h, size_t needed_capacity) {
  if (h->capacity < needed_capacity) {
    size_t new_capacity = h->capacity == 0 ? 8 : h->capacity;
    while (new_capacity < needed_capacity) {
      new_capacity *= 2;
    }
    action *new_actions = realloc(h->actions, new_capacity * sizeof(action));
    if (new_actions == NULL) {
      perror("realloc history failed");
      exit(EXIT_FAILURE);
    }
    h->actions = new_actions;
    h->capacity = new_capacity;
  }
}

void clear_history(history *h){
    for (size_t i = 0; i < h->count; ++i){
        free_action(&h->actions[i]);
    }
    h->count = 0;
}