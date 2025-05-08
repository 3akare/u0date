#include "main.h"

EditorMode mode = NORMAL;

int main(int argc, const char *argv[]) {
  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);

  buffer editor_buffer;
  init_buffer(&editor_buffer);

  size_t cursor_y = 0;
  size_t cursor_x = 0;

  int ch;
  int exit = 0;
  int win_max_y, win_max_x;
  getmaxyx(stdscr, win_max_y, win_max_x);

  if (argc != 2 || argv[1] == NULL) {
    endwin();
    printf("Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    read_file_into_buffer(argv[1], &editor_buffer);
    if (editor_buffer.num_rows > 0) {
      cursor_y = editor_buffer.num_rows - 1;
      cursor_x = editor_buffer.rows[cursor_y].size;
    } else {
      cursor_y = 0;
      cursor_x = 0;
    }
  }

  display_buffer(&editor_buffer, mode);
  move(cursor_y, cursor_x);

  while (!exit) {
    ch = getch();
    getmaxyx(stdscr, win_max_y, win_max_x);
    switch (mode) {
      case NORMAL:
        if (ch == 'q')
          exit = 1;
        else if (ch == ctrl('s')) {
          save_file_from_buffer(argv[1], &editor_buffer);
          mvprintw(win_max_y - 2, win_max_x - strlen("saved! "), "Saved!");
          mvprintw(win_max_y - 2, 0, stringify_mode(mode));
          move(cursor_y, cursor_x);
        }
        switch (ch) {
          case 'i':
            mode = INSERT;
            display_buffer(&editor_buffer, mode);
            move(cursor_y, cursor_x);
            break;
          case KEY_UP:
          case 'k':
            if (cursor_y > 0) {
              cursor_y--;
              cursor_x = min(cursor_x, editor_buffer.rows[cursor_y].size);
              move(cursor_y, cursor_x);
            }
            break;
          case KEY_DOWN:
          case 'j':
            if (cursor_y < editor_buffer.num_rows - 1) {
              cursor_y++;
              cursor_x = min(cursor_x, editor_buffer.rows[cursor_y].size);
              move(cursor_y, cursor_x);
            }
            break;
          case KEY_LEFT:
          case 'h':
            if (cursor_x > 0) {
              cursor_x--;
              move(cursor_y, cursor_x);
            } else if (cursor_y > 0) {
              cursor_y--;
              cursor_x = editor_buffer.rows[cursor_y].size;
              move(cursor_y, cursor_x);
            }
            break;
          case KEY_RIGHT:
          case 'l':
            if (cursor_y < editor_buffer.num_rows) {
              if (cursor_x < editor_buffer.rows[cursor_y].size) {
                cursor_x++;
                move(cursor_y, cursor_x);
              } else if (cursor_y < editor_buffer.num_rows - 1) {
                cursor_y++;
                cursor_x = 0;
                move(cursor_y, cursor_x);
              }
            }
            break;
        }
        break;
      case INSERT:
        if (ch == ESC) {
          mode = NORMAL;
          mvprintw(win_max_y - 2, win_max_x - strlen("       "), "       ");
          display_buffer(&editor_buffer, mode);
          move(cursor_y, cursor_x);
        } else if (ch == BACKSPACE || ch == KEY_BACKSPACE) {
          if (cursor_x > 0) {
            delete_char_in_row(&editor_buffer, cursor_y, cursor_x - 1);
            cursor_x--;
            display_buffer(&editor_buffer, mode);
            move(cursor_y, cursor_x);
          } else if (cursor_y > 0) {
            size_t prev_line_len = editor_buffer.rows[cursor_y - 1].size;
            join_rows(&editor_buffer, cursor_y - 1, cursor_y);
            cursor_y--;
            cursor_x = prev_line_len;
            display_buffer(&editor_buffer, mode);
            move(cursor_y, cursor_x);
          }
        } else if (ch == '\n' || ch == '\r') {
          split_row(&editor_buffer, cursor_y, cursor_x);
          cursor_y++;
          cursor_x = 0;
          display_buffer(&editor_buffer, mode);
          move(cursor_y, cursor_x);
        } else if (ch >= 32 && ch <= 126) {
          insert_char_in_row(&editor_buffer, cursor_y, cursor_x, ch);
          cursor_x++;
          display_buffer(&editor_buffer, mode);
          move(cursor_y, cursor_x);
        }
        break;
    }
  }
  free_buffer(&editor_buffer);
  endwin();
  return 0;
}