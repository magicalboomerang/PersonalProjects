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

const iPoint_2D ZERO_2D = (iPoint_2D){
	.X = 0,
	.Y = 0
};

//The display structure.
/*Summary: In simplest terms, The Display object holds structurally global
data and a list of GUI's, called Screens, to be shown to the user.
Screens hold all of the renderable data in a two tiered organization
of Layers and Images.

Display operates as a *single* object to handle all console output.
	It also holds certain global attributes. It organizes several Screen
	structures.

Screens are intended to be shown one at a time. They act as a container for layers.
	Screen objects are *always* the same resolution as the Display object.

The Layers are objects that primarily act as a way to organize draw order,
but also to organize related GUI elements, called Images.

Images are the basis for the GUI. They are character strings and
	relevant data for it's accurate display. Currently the library only
	handles rectangular images, but I am uncertain there is any value
	for any other geometry.

Theoretically this could be done dynamically and create a more
real time ASCII interface. That's a more ambitious solution
for another library.
 */

typedef struct Image{
	void *sImageFormedData; // The memory block of the characters of this image
	iPoint_2D ipLocation;//The location of this image relative to the parent LAYER.
	iPoint_2D ipDimensions;// Dimensions of the image.
	int iDisplaySize;//Used frequently for move ops. Precalculated for efficiency.
	char bIsVisible;// Boolean indicating whether to draw or not
} Image;

typedef struct Layer{
	Image **pImages; // Images contained in this layer.
	iPoint_2D ipLocation; // The location of this layer relative to it's screen.
	iPoint_2D ipDimensions;// The dimensions of the layer
	int iDisplaySize;
	char bIsVisible;
	char cImageCount;
	char cImageMaxCount;
} Layer;

typedef struct Screen{ // Dimensions Not neccessary, as they 
	Layer **pLayers;
//	char *(*InputValidatorFunction)(char *); // Removed for now. will likely separate 
//	void (*ValidatorFailureFunction)(void);  // I/O from this entirely.
//	iPoint_2D iCursorLocation;
	char bIsVisible;
	char cLayerCount;
	char cLayerMaxCount;
} Screen;

typedef struct Display{
	Image *ScreenLiteral;
	Screen **pScreens;
	Layer *pSystemLayer;
	iPoint_2D ipResolution;
	char cScreenCount;
	char cScreenMaxCount;
//	char cTransparentCharacter;
// Still idea stage. May require modifying the mem*****_2D functions into new specialized functions. A good idea for snazzy graphics, but may not be worth the time investment. This character will be universally not drawn, meaning the character 'underneath' will be preserved.
} Display;

// Function definitions
Image *InitializeImage(void *sImageFormedData);
Layer *InitializeLayer(void *sLayerFormedData);
void ReleaseImage(Image *iTargetImage);
void ReleaseLayer(Layer *lTargetLayer);
Display *UpdateDisplay(Display *pDisplay);
Image *UpdateScreen(Image *pDisplayImage, Screen *pScreen);
Image *UpdateLayer(Image *pDisplayImage, Layer *pLayer);
Image *UpdateImage(Image *pDisplayImage, Image *pImage, iPoint_2D *iLocationOffset);
void PrintImage(Image *pImage);

// 2D memory manipulation
void *memwrite_2D(void *pDestination, void *pSource, const iPoint_2D *iDestDimensions, const iPoint_2D *iSourceDimensions);
void *memread_2D(void *pDestination, void *pSource, const iPoint_2D *iDestDimensions, const iPoint_2D *iSourceDimensions);
void *memcpy_2D(void *pDestination, void *pSource, iPoint_2D *iDestLocation, iPoint_2D *iDestDimensions, iPoint_2D *iSourceLocation, iPoint_2D *iSourceDimensions, iPoint_2D *iCopyBlockDimensions);

// Display functions
void ClearConsole(void);
void setCursorLocation(iPoint_2D *TargetLocation);

// Source files. Dependent on above Libraries.
#include "ScreenString.c"

#endif