#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

int main(){
	char InputHolder[64];
	
	initscr();
	clear();
	
	addstr("I like big butts and I cannot lie.\n");
	refresh();
	getstr(InputHolder);
	curs_set(0);
	addstr("Great hunky munky! You entered: ");
	addstr(InputHolder);
	addstr("\nIsn't that neat!");
	refresh();
	usleep(3000000);
	
	
	endwin();
	
	return 0;
}
