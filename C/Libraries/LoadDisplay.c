#include "ScreenString.h"
#include <unistd.h>

int main(int iArgumentCount, char *pArgumentList[]){
	FILE *GUIFile = fopen("TestDisplay.gui", "r");
	
	Display *TestDisplay = InitializeDisplay(GUIFile);
	
	fclose(GUIFile);
	
	UpdateDisplay(TestDisplay);
	
	sleep(5);
	
	TestDisplay->pScreens[0]->bIsVisible = 0;
	
	UpdateDisplay(TestDisplay);
	
	ReleaseDisplay(TestDisplay);
	
	return 0;
}
