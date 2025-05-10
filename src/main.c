#include "main.h"

extern EditorMode mode;
extern history undo_history;
extern history redo_history;

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
        switch (ch) {
          case UNDO_KEY: {
            action action_to_undo;
            init_action(&action_to_undo);
            if (pop_action(&undo_history, &action_to_undo)) {
              switch (action_to_undo.type) {
                case ACTION_INSERT_CHAR:
                  delete_char_in_row(&editor_buffer, action_to_undo.row_index,
                                     action_to_undo.col_index);
                  cursor_y = action_to_undo.row_index;
                  cursor_x = action_to_undo.col_index;
                  break;
                case ACTION_DELETE_CHAR:
                  insert_char_in_row(&editor_buffer, action_to_undo.row_index,
                                     action_to_undo.col_index,
                                     action_to_undo.data[0]);
                  cursor_y = action_to_undo.row_index;
                  cursor_x = action_to_undo.col_index + 1;
                  break;
                case ACTION_INSERT_ROW:
                  delete_row(&editor_buffer, action_to_undo.row_index);
                  cursor_y = action_to_undo.row_index > 0
                                 ? action_to_undo.row_index - 1
                                 : 0;
                  cursor_x = editor_buffer.num_rows > 0
                                 ? editor_buffer.rows[cursor_y].size
                                 : 0;
                  break;
                case ACTION_DELETE_ROW: {
                  insert_row(&editor_buffer, action_to_undo.row_index);
                  row *new_row = &editor_buffer.rows[action_to_undo.row_index];
                  ensure_row_capacity(new_row, action_to_undo.data_size);
                  memcpy(new_row->contents, action_to_undo.data,
                         action_to_undo.data_size);
                  new_row->size = action_to_undo.data_size;
                  new_row->contents[new_row->size] = '\0';
                  cursor_y = action_to_undo.row_index;
                  cursor_x = 0;
                  break;
                }
                case ACTION_SPLIT_ROW:
                  join_rows(&editor_buffer, action_to_undo.row_index,
                            action_to_undo.row_index + 1);
                  cursor_y = action_to_undo.row_index;
                  cursor_x = action_to_undo.col_index;
                  break;
                case ACTION_JOIN_ROWS: {
                  split_row(&editor_buffer, action_to_undo.row_index,
                            action_to_undo.original_row1_size);
                  row *new_row =
                      &editor_buffer.rows[action_to_undo.row_index + 1];
                  ensure_row_capacity(new_row, action_to_undo.data_size);
                  memcpy(new_row->contents, action_to_undo.data,
                         action_to_undo.data_size);
                  new_row->size = action_to_undo.data_size;
                  new_row->contents[new_row->size] = '\0';
                  cursor_y = action_to_undo.row_index + 1;
                  cursor_x = 0;
                  break;
                }
              }
            }
            if (editor_buffer.num_rows == 0) {
              cursor_y = 0;
              cursor_x = 0;
            } else {
              cursor_x = min(cursor_x, editor_buffer.rows[cursor_y].size);
            }
            action action_for_redo = action_to_undo;
            if (action_to_undo.data_size > 0 && action_to_undo.data) {
              action_for_redo.data = malloc(action_to_undo.data_size + 1);
              if (action_for_redo.data) {
                memcpy(action_for_redo.data, action_to_undo.data,
                       action_to_undo.data_size);
                action_for_redo.data[action_to_undo.data_size] = '\0';
              } else {
                action_for_redo.data_size = 0;
                action_for_redo.data = NULL;
                perror("malloc for redo action data failed");
              }
            } else {
              action_for_redo.data = NULL;
              action_for_redo.data_size = 0;
            }
            push_action(&redo_history, &action_for_redo);
            free_action(&action_to_undo);
            display_buffer(&editor_buffer, mode);
            mvprintw(win_max_y - 2, win_max_x - strlen("saved! "), "Undo!");
            move(cursor_y, cursor_x);
            break;
          }
          case REDO_KEY: {
            action action_to_redo;
            init_action(&action_to_redo);
            if (pop_action(&redo_history, &action_to_redo)) {
              switch (action_to_redo.type) {
                case ACTION_INSERT_CHAR:
                  insert_char_in_row(&editor_buffer, action_to_redo.row_index,
                                     action_to_redo.col_index,
                                     action_to_redo.data[0]);
                  cursor_y = action_to_redo.row_index;
                  cursor_x = action_to_redo.col_index + 1;
                  break;
                case ACTION_DELETE_CHAR:
                  delete_char_in_row(&editor_buffer, action_to_redo.row_index,
                                     action_to_redo.col_index);
                  cursor_y = action_to_redo.row_index;
                  cursor_x = action_to_redo.col_index;
                  break;
                case ACTION_INSERT_ROW:
                  insert_row(&editor_buffer, action_to_redo.row_index);
                  cursor_y = action_to_redo.row_index;
                  cursor_x = 0;
                  break;
                case ACTION_DELETE_ROW: {
                  delete_row(&editor_buffer, action_to_redo.row_index);
                  cursor_y = action_to_redo.row_index > 0
                                 ? action_to_redo.row_index - 1
                                 : 0;
                  cursor_x =
                      editor_buffer.num_rows > 0
                          ? min(cursor_x, editor_buffer.rows[cursor_y].size)
                          : 0;
                  break;
                }
                case ACTION_SPLIT_ROW:
                  split_row(&editor_buffer, action_to_redo.row_index,
                            action_to_redo.col_index);
                  row *new_row =
                      &editor_buffer.rows[action_to_redo.row_index + 1];
                  ensure_row_capacity(new_row, action_to_redo.data_size);
                  memcpy(new_row->contents, action_to_redo.data,
                         action_to_redo.data_size);
                  new_row->size = action_to_redo.data_size;
                  new_row->contents[new_row->size] = '\0';
                  cursor_y = action_to_redo.row_index + 1;
                  cursor_x = 0;
                  break;

                case ACTION_JOIN_ROWS:
                  join_rows(&editor_buffer, action_to_redo.row_index,
                            action_to_redo.row_index + 1);
                  cursor_y = action_to_redo.row_index;
                  cursor_x = action_to_redo.original_row1_size;
                  break;
              }
              if (editor_buffer.num_rows == 0) {
                cursor_y = 0;
                cursor_x = 0;
              } else {
                cursor_x = min(cursor_x, editor_buffer.rows[cursor_y].size);
              }
              action action_for_undo = action_to_redo;
              if (action_to_redo.data_size > 0 && action_to_redo.data) {
                action_for_undo.data = malloc(action_to_redo.data_size + 1);
                if (action_for_undo.data) {
                  memcpy(action_for_undo.data, action_to_redo.data,
                         action_to_redo.data_size);
                  action_for_undo.data[action_to_redo.data_size] = '\0';
                } else {
                  action_for_undo.data_size = 0;
                  action_for_undo.data = NULL;
                  perror("malloc for undo action data failed");
                }
              } else {
                action_for_undo.data = NULL;
                action_for_undo.data_size = 0;
              }
              push_action(&undo_history, &action_for_undo);
              free_action(&action_to_redo);
              mvprintw(win_max_y - 2, win_max_x - strlen("saved! "), "Redo!");
              display_buffer(&editor_buffer, mode);
              move(cursor_y, cursor_x);
            }
            break;
          }
          case SAVE_KEY:
            save_file_from_buffer(argv[1], &editor_buffer);
            mvprintw(win_max_y - 2, win_max_x - strlen("saved! "), "Saved!");
            mvprintw(win_max_y - 2, 0, stringify_mode(mode));
            move(cursor_y, cursor_x);
            break;
          case 'q':
            exit = 1;
            break;
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
            int deleted_char = editor_buffer.rows[cursor_y].contents[cursor_x];
            delete_char_in_row(&editor_buffer, cursor_y, cursor_x - 1);

            action new_action;
            init_action(&new_action);
            new_action.type = ACTION_DELETE_CHAR;
            new_action.row_index = cursor_y;
            new_action.col_index = cursor_x - 1;

            new_action.data = malloc(2);
            if (new_action.data) {
              new_action.data[0] = deleted_char;
              new_action.data[1] = '\0';
              new_action.data_size = 1;
            } else {
              perror("malloc for action data failed");
            }
            push_action(&undo_history, &new_action);
            clear_history(&redo_history);
            free_action(&new_action);

            cursor_x--;
            display_buffer(&editor_buffer, mode);
            move(cursor_y, cursor_x);
          } else if (cursor_y > 0) {
            row *r_to_delete = &editor_buffer.rows[cursor_y];
            char *deleted_row_content = NULL;
            size_t deleted_row_size = 0;
            if (r_to_delete->size > 0) {
              deleted_row_content = malloc(r_to_delete->size + 1);
              if (deleted_row_content) {
                memcpy(deleted_row_content, r_to_delete->contents,
                       r_to_delete->size + 1);
                deleted_row_size = r_to_delete->size;
              } else {
                perror("malloc for action data failed");
              }
            }
            size_t original_r1_size = editor_buffer.rows[cursor_y - 1].size;
            join_rows(&editor_buffer, cursor_y - 1, cursor_y);

            action new_action;
            init_action(&new_action);
            new_action.type = ACTION_JOIN_ROWS;
            new_action.row_index = cursor_y - 1;
            new_action.col_index = original_r1_size;
            new_action.data = deleted_row_content;
            new_action.data_size = deleted_row_size;
            new_action.original_row1_size = original_r1_size;

            push_action(&undo_history, &new_action);
            clear_history(&redo_history);
            free_action(&new_action);

            cursor_y--;
            cursor_x = original_r1_size;
            display_buffer(&editor_buffer, mode);
            move(cursor_y, cursor_x);
          }
        } else if (ch == '\n' || ch == '\r') {
          size_t original_col_index = cursor_x;
          split_row(&editor_buffer, cursor_y, cursor_x);

          row *new_r = &editor_buffer.rows[cursor_y + 1];
          char *moved_content = NULL;
          size_t moved_size = 0;
          if (new_r->size > 0) {
            moved_content = malloc(new_r->size + 1);
            if (moved_content) {
              memcpy(moved_content, new_r->contents, new_r->size + 1);
              moved_size = new_r->size;
            } else {
              perror("malloc for action data failed");
            }
          }

          action new_action;
          init_action(&new_action);
          new_action.type = ACTION_SPLIT_ROW;
          new_action.row_index = cursor_y;
          new_action.col_index = original_col_index;
          new_action.data = moved_content;
          new_action.data_size = moved_size;

          push_action(&undo_history, &new_action);
          clear_history(&redo_history);
          free_action(&new_action);

          cursor_y++;
          cursor_x = 0;
          display_buffer(&editor_buffer, mode);
          move(cursor_y, cursor_x);
        } else if (ch >= 32 && ch <= 126) {
          int inserted_char = ch;
          insert_char_in_row(&editor_buffer, cursor_y, cursor_x, inserted_char);

          action new_action;
          init_action(&new_action);
          new_action.type = ACTION_INSERT_CHAR;
          new_action.row_index = cursor_y;
          new_action.col_index = cursor_x;
          new_action.data = malloc(2);
          if (new_action.data) {
            new_action.data[0] = inserted_char;
            new_action.data[1] = '\0';
            new_action.data_size = 1;
          } else {
            perror("malloc for action data failed");
          }

          push_action(&undo_history, &new_action);
          clear_history(&redo_history);
          free_action(&new_action);

          cursor_x++;
          display_buffer(&editor_buffer, mode);
          move(cursor_y, cursor_x);
        }
        break;
    }
  }
  free_buffer(&editor_buffer);
  free_history(&undo_history);
  free_history(&redo_history);
  endwin();
  return 0;
}