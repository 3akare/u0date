#include "main.h"

Mode mode = NORMAL;

int main(int argc, char* argv[]){
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);

    int ch, x, y, max_x, max_y;
    int exit = 0;

    getmaxyx(stdscr, max_y, max_x);
    getyx(stdscr, y, x);

    mvprintw(max_y - 1, 0, stringify_mode(mode));
    move(y, x);

    while ((ch = getch())){	
	switch(mode){
	    case NORMAL: 
		if (ch == 'i'){
		    mode = INSERT;
		    mvprintw(max_y - 1, 0, stringify_mode(mode));
		    move(y, x);
		}
		else if (ch == 'q') exit = 1;
		break;
	    case INSERT:
		if (ch == ESC){
		    mode = NORMAL;
		    mvprintw(max_y - 1, 0, stringify_mode(mode));
		    move(y, x);
		}
		else {
		    if (ch == BACKSPACE || ch == KEY_BACKSPACE){
			getyx(stdscr, y, x);
			move(y, x - 1);
			delch();
		    }
		    else addch(ch); 
		}
		break;
	}
	if (exit) break;
	getyx(stdscr, y, x);
    }
    endwin();
    return 0;
}
