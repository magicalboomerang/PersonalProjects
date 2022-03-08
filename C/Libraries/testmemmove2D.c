#include "ScreenString.h"

#define WIDTHDETLA (DSTWIDTH - SRCWIDTH)
#define DSTWIDTH 50
#define SRCWIDTH 3

int main(){
	// Character blocks of memory to be modified
	char pDestBlock[DSTWIDTH * DSTWIDTH];
	char pSourceBlock[SRCWIDTH * SRCWIDTH];
	int PrintX = 0, PrintY = 0;
	int Xdelta = -1, Ydelta = -1;
	int LoopCount = 0;
	
	// Fill each block with distinct characters
	memset(pDestBlock, '0', DSTWIDTH * DSTWIDTH * sizeof(char));
	memset(pSourceBlock, '-', SRCWIDTH * SRCWIDTH * sizeof(char));
	
	// Set up dimensions of blocks
	struct iPoint_2D iDestDimensions;
		iDestDimensions.X = DSTWIDTH;
		iDestDimensions.Y = DSTWIDTH;
	
	struct iPoint_2D iSourceDimensions;
		iSourceDimensions.X = SRCWIDTH;
		iSourceDimensions.Y = SRCWIDTH;
	
	while(1){
		// Test moving an array in
		memwrite_2D( &pDestBlock[PrintY * DSTWIDTH + PrintX], &pSourceBlock, &iDestDimensions, &iSourceDimensions);

		ClearConsole();
		printf("%d\n", LoopCount++);
		printBlock(pDestBlock, &iDestDimensions);
		memset(pDestBlock, '0', DSTWIDTH * DSTWIDTH * sizeof(char));
		
		if(PrintX >= WIDTHDETLA || PrintX <= 0)
			Xdelta *= -1;
		if(PrintY >= WIDTHDETLA || PrintY <= 0)
			Ydelta *= -1;
		
		PrintX += Xdelta;
		PrintY += Ydelta;
	}
	return 0;
}
