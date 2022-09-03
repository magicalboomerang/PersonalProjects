#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main(int iArgumentCount, char *pArgumentList[]){
	// Initialize
	initscr();
	
	// Clear display
	clear();
	
	// Update the display
	refresh();
	
	// Pause to show we have created a blank open screen.
	sleep(5);
	
	// Stop curses
	endwin();
	
	return 0;
}
