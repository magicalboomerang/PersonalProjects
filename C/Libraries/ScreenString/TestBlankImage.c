#include <stdlib.h>
#include <stdio.h>
#include "ScreenString.h"

int main(int iArgumentCount, char *pArgumentList[]){
	iPoint_2D iLocation = ZERO_2D;
	iPoint_2D iDimensions = {.X = 10, .Y = 10};
	
	Image *BlankImage = InitializeBlankImage(&iLocation, &iDimensions);
	
	PrintImage(BlankImage);
	
	ReleaseImage(BlankImage);
	
	return 0;
}
