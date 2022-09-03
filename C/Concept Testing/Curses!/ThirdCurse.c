#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>

typedef struct iPoint_2D {
	int X;
	int Y;
} iPoint_2D;

int main(int iArgumentCount, char *pArgumentList[]){
	iPoint_2D iBallLocation = {.X = 1, .Y = 1};
	iPoint_2D iBallDelta = {.X = 1, .Y = 1};
	
	// Initialize
	initscr();
	
	for(int iBounceCycles = 1000; iBounceCycles > 0; iBounceCycles--){
		clear();
		mvaddch(iBallLocation.X, iBallLocation.Y,'O');
		
		if(iBallLocation.X >=COLS/4 || iBallLocation.X <= 0)
			iBallDelta.X *= -1;
		if(iBallLocation.Y >= COLS/3 || iBallLocation.Y <= 0)
			iBallDelta.Y *= -1;
		
		iBallLocation.X += iBallDelta.X;
		iBallLocation.Y += iBallDelta.Y;
		
		refresh();
		usleep(10000);
	}
	
	endwin();
	
	return 0;
}
