#include "main.h"

Mode mode = NORMAL;

int main(int argc, char *argv[]) {
	initscr();
	noecho();
	raw();
	keypad(stdscr, TRUE);

	char *buffer = malloc(sizeof(char) * 1024);
	size_t buffer_s = 0;

	int ch, x, y, max_x, max_y;
	int exit = 0;

	getmaxyx(stdscr, max_y, max_x);
	getyx(stdscr, y, x);

	mvprintw(max_y - 1, 0, stringify_mode(mode));
	move(y, x);

	while ((ch = getch())) {
		switch (mode) {
			case NORMAL:
				if (ch == 'q')
					exit = 1;
				else if (ch == ctrl('s')) {
					FILE *file = fopen("test.txt", "wb");
					fwrite(buffer, 1, buffer_s, file);
					fclose(file);
					exit = 1;
				}
				switch (ch) {
					case 'i':
						mode = INSERT;
						mvprintw(max_y - 1, 0, stringify_mode(mode));
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
					mvprintw(max_y - 1, 0, stringify_mode(mode));
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
	}
	endwin();
	return 0;
}
