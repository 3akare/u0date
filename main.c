#include <stdio.h>
#include <ncurses.h>
#include <strings.h>

#define ctrl(x) ((x) & 0x1f)
#define ESC 27
#define BACKSPACE 127

typedef enum {
    NORMAL,
    INSERT
} Mode;

Mode mode = NORMAL;

const char* stringify_mode(Mode mode){
    switch(mode){
	case NORMAL: return "-- NORMAL --";
	case INSERT: return "-- INSERT --";
	default: return "-- NORMAL --";
    }
}

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
