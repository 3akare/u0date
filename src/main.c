#include "main.h"

// default Editor mode
EditorMode mode = NORMAL;

int main(int argc, const char *argv[]) {
  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);

  size_t buffer_s = 0;

  // cursor position
  size_t cursor_y = 0;
  size_t cursor_x = 0;

  int exit = 0;
  int ch;
  int max_x, max_y;
  char *buffer = malloc(sizeof(char) * BUFSIZ);

  getmaxyx(stdscr, max_y, max_x);
  getyx(stdscr, cursor_y, cursor_x);

  mvprintw(max_y - 2, 0, stringify_mode(mode));
  move(cursor_y, cursor_x);

  if (argc != 2 || argv[1] == NULL) {
    endwin();
    printf("Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  } else
    buffer_s = read_from_file(argv[1], buffer, buffer_s);

  while ((ch = getch())) {
    switch (mode) {
      case NORMAL:
        if (ch == 'q')
          exit = 1;
        else if (ch == ctrl('s')) {
          save_to_file(argv[1], buffer, buffer_s);
          mvprintw(max_y - 2, max_x - strlen("saved! "), "Saved!");
          move(cursor_y, cursor_x);
        }
        switch (ch) {
          case 'i':
            mode = INSERT;
            mvprintw(max_y - 2, 0, stringify_mode(mode));
            mvprintw(max_y - 2, max_x - strlen("saved! "), "        ");
            if (buffer_s > 0)
              move(cursor_y, buffer_s);
            else
              move(cursor_y, cursor_x);
            break;
          case KEY_UP:
          case 'k':
            move(--cursor_y, cursor_x);
            break;
          case KEY_DOWN:
          case 'j':
            move(++cursor_y, cursor_x);
            break;
          case KEY_LEFT:
          case 'h':
            move(cursor_y, --cursor_x);
            break;
          case KEY_RIGHT:
          case 'l':
            move(cursor_y, ++cursor_x);
            break;
        }
        break;
      case INSERT:
        if (ch == ESC) {
          mode = NORMAL;
          mvprintw(max_y - 2, 0, stringify_mode(mode));
          move(cursor_y, cursor_x);
        } else {
          if (ch == BACKSPACE || ch == KEY_BACKSPACE) {
            getyx(stdscr, cursor_y, cursor_x);
            move(cursor_y, cursor_x - 1);
            delch();
            if (buffer_s > 0) buffer[buffer_s--] = ' ';
          } else {
            addch(ch);
            buffer[buffer_s++] = ch;
          };
        }
        break;
    }
    if (exit) break;
    getyx(stdscr, cursor_y, cursor_x);
    getmaxyx(stdscr, max_y, max_x);
  }
  endwin();
  return 0;
}
