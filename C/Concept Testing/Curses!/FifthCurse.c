#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <term.h>
#include <sys/wait.h>
#define TERMINALCOUNT 3

typedef struct iPoint_2D {
	int X;
	int Y;
} iPoint_2D;

int main(/*int iArgumentCount, char *pArgumentList[]*/){
	iPoint_2D iBallLocation = {.X = 1, .Y = 1};
	iPoint_2D iBallDelta = {.X = 1, .Y = 1};
	FILE *pTerminalFile[TERMINALCOUNT];
	SCREEN *pTerminal;
	
	char sTerminalLocation[16];
	int iPID, iCPID, iTerminalIndex, iChildCount = 0;
	
	// Open the terminal files
	for(iTerminalIndex = 0; iTerminalIndex < TERMINALCOUNT; iTerminalIndex++){
		sprintf((char *)sTerminalLocation, "/dev/pts/%d", iTerminalIndex);
		pTerminalFile[iTerminalIndex] = fopen(sTerminalLocation, "r+");
		if(!pTerminalFile[iTerminalIndex]){
			printf("Terminal %d not found.\n", iTerminalIndex);
			
			
			// Close files that DID open
			for(;iTerminalIndex >= 0; iTerminalIndex--)
				fclose(pTerminalFile[iTerminalIndex]);
			
			return 1;
		} else {
			iChildCount++;
		}
	}
	
	// The terminals exist and are open!
	// Fork off and let the terminals sub-processes do their thing.
	do{
		iTerminalIndex--;
		iPID = fork();
	} while(iTerminalIndex > 0 && iPID != 0);
	
	if(iPID == 0){ // I'm a child
		// Create the new screen
		pTerminal = newterm((char *)0, pTerminalFile[iTerminalIndex], NULL);
		set_term(pTerminal);
		
		curs_set(0);
		
		iBallDelta.Y += iTerminalIndex;
		
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
		
		fclose(pTerminalFile[iTerminalIndex]);
		
		printf("This ball is all done: %d\n", getpid());
	} else { //I'm a parent!
		do{
			iCPID = wait(NULL);
			iChildCount--;

		} while(iChildCount > 0);
		printf("All my balls are bounced out :( !\n");
	}
	return 0;
}
