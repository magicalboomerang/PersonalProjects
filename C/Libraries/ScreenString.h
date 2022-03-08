// ScreenString.h
#ifndef SCREENSTRING_H
#define SCREENSTRING_H

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs
typedef struct iPoint_2D {
	int X;
	int Y;
} iPoint_2D;

typedef struct Image{
	char *pTextLiteral;
	iPoint_2D iLocation;
	iPoint_2D iDimensions;
	int iLinearSize;
	char bIsVisible;
} Image;

typedef struct Layer{
	Image *Images;
	iPoint_2D iLocation;
	iPoint_2D iDimensions;
	int iLinearSize;
	char bIsVisible;
	char cImageCount;
	char cImageMaxCount;
} Layer;

typedef struct Screen{
	Layer *Background;
	Layer *Layers;
	char *(*InputValidatorFunction)(char *);
	void (*ValidatorFailureFunction)();
	iPoint_2D iCursorLocation;
	char cLayerCount;
	char cLayerMaxCount;
} Screen;

typedef struct Display{
	Screen *Screens;
	Layer *SystemLayer;
	iPoint_2D Resolution;
	char cActiveScreenIndex;
	char cScreenCount;
	char cScreenMaxCount;
} Display;

// Function definitions
// 2D memory manipulation
void * memwrite_2D(void *pDestination, void *pSource, iPoint_2D *iDestDimensions, iPoint_2D *iSourceDimensions);
void * memread_2D(void *pDestination, void *pSource, iPoint_2D *iDestDimensions, iPoint_2D *iSourceDimensions);
void * memcpy_2D(void *pDestination, void *pSource, iPoint_2D *iDestLocation, iPoint_2D *iDestDimensions, iPoint_2D *iSourceLocation, iPoint_2D *iSourceDimensions, iPoint_2D *iCopyBlockDimensions);

// Display functions
void ClearConsole(void);
void setCursorLocation(iPoint_2D *TargetLocation);

// Temporary functions
void printBlock(void *pMemoryBlock, iPoint_2D *BlockDimensions);

// Source files. Dependent on above Libraries.
#include "ScreenString.c"

#endif