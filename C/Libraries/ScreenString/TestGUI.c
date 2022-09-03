#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "ScreenString.h"

int main(int iArgumentCount, char *pArgumentList[]){
	FILE *pGUIFILE = fopen("Wordle.gui", "rb");
	
	Display *pTestDisplay = LoadDisplay(pGUIFILE);
	
	for(int iScreenIndex = 0; iScreenIndex < 2; iScreenIndex++){
	UpdateDisplay(pTestDisplay);
	
	sleep(5);
	
	pTestDisplay->pScreens[iScreenIndex]->bIsVisible = 0;
	}
	
	
	
	ReleaseDisplay(pTestDisplay);
	
	return 0;
}
