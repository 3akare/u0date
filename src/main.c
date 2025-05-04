#include "main.h"

// default Editor mode
EditorMode mode = NORMAL;

int main(int argc, const char *argv[]) {
	initscr();
	noecho();
	raw();
	keypad(stdscr, TRUE);

	int exit = 0;
	size_t buffer_s = 0;
	int ch, x, y, max_x, max_y;
	char *buffer = malloc(sizeof(char) * BUFSIZ);

	getmaxyx(stdscr, max_y, max_x);
	getyx(stdscr, y, x);

	mvprintw(max_y - 2, 0, stringify_mode(mode));
	move(y, x);

	if (argc != 2 || argv[1] == NULL) {
		printf("Error: add a filename");
		return 0;
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
					move(y, x);
				}
				switch (ch) {
					case 'i':
						mode = INSERT;
						mvprintw(max_y - 2, 0, stringify_mode(mode));
						mvprintw(max_y - 2, max_x - strlen("saved! "), "        ");
						if (buffer_s > 0)
							move(y, buffer_s);
						else
							move(y, x);
						break;
					case KEY_UP:
					case 'k':
						move(--y, x);
						break;
					case KEY_DOWN:
					case 'j':
						move(++y, x);
						break;
					case KEY_LEFT:
					case 'h':
						move(y, --x);
						break;
					case KEY_RIGHT:
					case 'l':
						move(y, ++x);
						break;
				}
				break;
			case INSERT:
				if (ch == ESC) {
					mode = NORMAL;
					mvprintw(max_y - 2, 0, stringify_mode(mode));
					move(y, x);
				} else {
					if (ch == BACKSPACE || ch == KEY_BACKSPACE) {
						getyx(stdscr, y, x);
						move(y, x - 1);
						delch();
						buffer[buffer_s--] = ' ';
					} else {
						addch(ch);
						buffer[buffer_s++] = ch;
					};
				}
				break;
		}
		if (exit) break;
		getyx(stdscr, y, x);
		getmaxyx(stdscr, max_y, max_x);
	}
	endwin();
	return 0;
}
