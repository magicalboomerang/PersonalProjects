#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main(int iArgumentCount, char *pArgumentList[]){
	// Initialize
	initscr();
	
	// Add a test line
	addstr("Printing on the basic display!");
	// Show the screen
	refresh();
	// Pause to see the state of the screen.
	sleep(5);
	
	// Clear display
	clear();
	// Print a post clear message
	addstr("Where'd everybody go!?!");
	// Update the display
	refresh();
	// Pause to show we have created a blank open screen.
	sleep(5);
	
	// Stop curses
	endwin();
	
	return 0;
}
