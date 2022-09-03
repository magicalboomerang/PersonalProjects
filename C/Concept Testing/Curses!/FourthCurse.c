#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <term.h>

typedef struct iPoint_2D {
	int X;
	int Y;
} iPoint_2D;

int main(/*int iArgumentCount, char *pArgumentList[]*/){
	iPoint_2D iBallLocation = {.X = 1, .Y = 1};
	iPoint_2D iBallDelta = {.X = 1, .Y = 1};
	FILE *pTerminalFile;
	SCREEN *pTerminal;
	
	// Open the terminal files
	pTerminalFile = fopen("/dev/pts/1", "r+");
	if(!pTerminalFile){
		printf("Terminal 1 not found.\n");
		return 1;
	}
	
	// Initialize
	//initscr(); Don't do this for multi-terminal mode, i think?
	
	// Create the new screen
	pTerminal = newterm((char *)0, pTerminalFile, NULL);
	set_term(pTerminal);
	
	for(int iBounceCycles = 1000; iBounceCycles > 0; iBounceCycles--){
		clear();
		mvaddch(iBallLocation.X, iBallLocation.Y,'O');
		
		if(iBallLocation.X >=COLS/3 || iBallLocation.X <= 0)
			iBallDelta.X *= -1;
		if(iBallLocation.Y >= COLS/4 || iBallLocation.Y <= 0)
			iBallDelta.Y *= -1;
		
		iBallLocation.X += iBallDelta.X;
		iBallLocation.Y += iBallDelta.Y;
		
		refresh();
		usleep(10000);
	}
	
	endwin();
	
	fclose(pTerminalFile);
	
	return 0;
}
