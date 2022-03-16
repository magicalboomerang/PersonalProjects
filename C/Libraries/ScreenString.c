#ifndef SCREENSTRING
#define SCREENSTRING

#include "ScreenString.h"

/* Plan of execution:
    1. 2d memory operations, and basic structs.
    2. Struct heirarchy
    4. Layer stuct + functions (since this is what will handle the images.)
    5. Base layer struct (zeroth layer)
    6. Screen struct + functions
    7. Display Objects
 */

// Functions
// Display Structure initializers.
/*Purpose: This takes a FULLY FORMED AND FORMATTED Image binary object
	and places it into memory. Must be deallocated with ReleaseImage();
Arguments: *pImageDataFile - The binary data to be convereted into
	an image object.
Returns: An Image pointer to the newly created image.
Bugs: None known.
*/
Image *InitializeImage(FILE *pImageDataFile){
	Image *pInitializer;
	int pImageDetails[4];
	
	if(!pImageDataFile){
		pInitializer = NULL;
	} else {
		// Note pImageDetails realligns potential misalignments.
		fread((void *)pImageDetails, sizeof(int), 4, pImageDataFile);
		
		// Make space for the image object, and set up its details
		pInitializer = (Image *)malloc(sizeof(Image));
		pInitializer->pLocation.X = pImageDetails[0];
		pInitializer->pLocation.Y = pImageDetails[1];
		pInitializer->pDimensions.X = pImageDetails[2];
		pInitializer->pDimensions.Y = pImageDetails[3];
		pInitializer->iDisplaySize = pInitializer->pDimensions.X * pInitializer->pDimensions.Y;
		pInitializer->bIsVisible = 1; // All images are visible by default, for usability.
		
		// Make space for the image data. Separating the memory blocks ensures dynamic image sizes. The sizeof operator needs to be changed for non-ASCII application.
		pInitializer->pImageFormedData = malloc(sizeof(char) * pInitializer->iDisplaySize);
		
		fread(pInitializer->pImageFormedData, sizeof(char), pInitializer->iDisplaySize, pImageDataFile);
	}
	return pInitializer;
}

// NOTE: THE IMAGE BLOCK DOES NOT READ CHARACTERS FOR DIMENSIONAL DATA. IT TREATS THE
// DATA AS BINARY, NOT CHARACTERS. This is intended to read data from file to initialize.
Layer *InitializeLayer(FILE *pLayerDataFile){
	int pLayerDetails[6], iImageIndex;
	Layer *pInitializer;
	
	if(!pLayerDataFile){
		return NULL;
	} else {
		// Allocate the space for this Layer
		pInitializer = (Layer *)malloc(sizeof(Layer));
		
		// Read the layer details.
		fread((void *)pLayerDetails, sizeof(int), 6, pLayerDataFile);
		
		// Add details to the newly created object.
		pInitializer->cImageMaxCount = Clamp(pLayerDetails[0], 0, IMAGECAP);
		pInitializer->cImageCount = Clamp(pLayerDetails[1], 0, pInitializer->cImageMaxCount);
		pInitializer->pLocation.X = pLayerDetails[2];
		pInitializer->pLocation.Y = pLayerDetails[3];
		pInitializer->pDimensions.X = pLayerDetails[4];
		pInitializer->pDimensions.Y = pLayerDetails[5];
		pInitializer->iDisplaySize = pInitializer->pDimensions.X * pInitializer->pDimensions.Y;
		pInitializer->bIsVisible = 1;
		
		// Allocate the image address space. Separating the Layer and Image address lists in memory allows us to resize the maximum image count during run-time.
		pInitializer->pImages = (Image **)malloc(sizeof(Image) * pInitializer->cImageMaxCount);
		
		// Using running total offsets, properly read and write the images
		// I think there is a better solution to be found.
		for( iImageIndex = 0; iImageIndex < pInitializer->cImageCount; iImageIndex++){
			// Pass the address of the first byte of image data
			pInitializer->pImages[iImageIndex] = InitializeImage(pLayerDataFile);
		}
	}
	return (Layer *)pInitializer;
}

/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
Screen *InitializeScreen(FILE *pScreenDataFile){
	int pScreenDetails[2], iLayerIndex;
	Screen *pInitializer;
	
	if(!pScreenDataFile){
		return NULL;
	} else {
		fread((void *)pScreenDetails, sizeof(int), 2, pScreenDataFile);
		
		// Allocate the space for this Layer
		pInitializer = (Screen *)malloc(sizeof(Screen));
		
		// Extract the data from the ASCII block
		pInitializer->cLayerMaxCount = Clamp(pScreenDetails[0], 0, LAYERCAP);
		pInitializer->cLayerCount = Clamp(pScreenDetails[1], 0, pInitializer->cLayerMaxCount);
		pInitializer->bIsVisible = 1;
		
		// Allocate the layer address space. Separating the Screen and Layer address lists in memory allows us to resize the maximum image count during run-time.
		pInitializer->pLayers = (Layer **)malloc(sizeof(Layer) * pInitializer->cLayerMaxCount);
		
		// Using running total offsets, properly read and write the images
		// I think there is a better solution to be found.
		for( iLayerIndex = 0; iLayerIndex < pInitializer->cLayerCount; iLayerIndex++){
			// Pass the address of the first byte of image data
			pInitializer->pLayers[iLayerIndex] = InitializeLayer(pScreenDataFile);
		}
	}
	return (Screen *)pInitializer;
}

/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
Note: There are excessive return NULL statements, since this object
	must be initialized correctly. Any failure should return all
	allocated memory to the system.
*/
Display *InitializeDisplay(FILE *pDisplayDataFile){
	int pDisplayDetails[4], iScreenIndex;
	Display *pInitializer;
	
	if(!pDisplayDataFile){
		return NULL;
	} else {
		fread((void *)pDisplayDetails, sizeof(int), 4, pDisplayDataFile);
		
		// Allocate the space for this Layer
		pInitializer = (Display *)malloc(sizeof(Display));
		if(!pInitializer)
			return NULL;
		
		// Initialize the display screen.
		pInitializer->pScreenLiteral = (Image *)malloc(sizeof(Image));
		if(!pInitializer->pScreenLiteral){
			free(pInitializer);
			return NULL;
		}
		// Extract the data from the read array.
		pInitializer->cScreenMaxCount = Clamp(pDisplayDetails[0], 0, SCREENCAP);
		pInitializer->cScreenCount = Clamp(pDisplayDetails[1], 0, pInitializer->cScreenMaxCount);
		pInitializer->pResolution.X = pDisplayDetails[2];
		pInitializer->pResolution.Y = pDisplayDetails[3];
		pInitializer->bClearOnUpdate = 1;
		
		// Manually set up the display image
		pInitializer->pScreenLiteral->pLocation.X = 0;
		pInitializer->pScreenLiteral->pLocation.Y = 0;
		pInitializer->pScreenLiteral->pDimensions.X = pInitializer->pResolution.X;
		pInitializer->pScreenLiteral->pDimensions.Y = pInitializer->pResolution.Y;
		pInitializer->pScreenLiteral->iDisplaySize = pInitializer->pScreenLiteral->pDimensions.X * pInitializer->pScreenLiteral->pDimensions.Y;
		pInitializer->pScreenLiteral->bIsVisible = 1;
		pInitializer->pScreenLiteral->pImageFormedData = malloc(sizeof(char) * pInitializer->pScreenLiteral->iDisplaySize);
		if(!pInitializer->pScreenLiteral->pImageFormedData){
			free(pInitializer->pScreenLiteral);
			free(pInitializer);
			return NULL;
		}
		// Blank the new screen.
		memset(pInitializer->pScreenLiteral->pImageFormedData, ' ', sizeof(char) * pInitializer->pScreenLiteral->iDisplaySize);
		
		// Allocate the layer address space. Separating the Screen and Layer address lists in memory allows us to resize the maximum image count during run-time.
		pInitializer->pScreens = (Screen **)malloc(sizeof(Screen) * pInitializer->cScreenMaxCount);
		if(!pInitializer->pScreens){
			free(pInitializer->pScreenLiteral->pImageFormedData);
			free(pInitializer->pScreenLiteral);
			free(pInitializer);
			return NULL;
		}
		
		// Initialize the system layer
		pInitializer->pSystemLayer = InitializeLayer(pDisplayDataFile);
		
		// Initialize all of the screens
		for( iScreenIndex = 0; iScreenIndex < pInitializer->cScreenCount; iScreenIndex++){
			// Pass the address of the first byte of image data
			pInitializer->pScreens[iScreenIndex] = InitializeScreen(pDisplayDataFile);
		}
	}
	return (Display *)pInitializer;
}

// Garbage collectors/object deallocation functions.
void ReleaseImage(Image *pTargetImage){
	if(pTargetImage->pImageFormedData) // If the pointer is not null...
		free(pTargetImage->pImageFormedData);// free the image data.
	free(pTargetImage);
}

void ReleaseLayer(Layer *pTargetLayer){
	int iImageRootIndex, iImageReferenceIndex;
	
	// If we have duplicate addresses, we must avoid double free calls
	// Iteratively look for duplicate addresses, if they are equal, set one to NULL.
	for(iImageRootIndex = 0; iImageRootIndex < pTargetLayer->cImageCount - 1; iImageRootIndex++)
		for(iImageReferenceIndex = iImageRootIndex + 1; iImageReferenceIndex < pTargetLayer->cImageCount; iImageReferenceIndex++)
			if(pTargetLayer->pImages[iImageRootIndex] == pTargetLayer->pImages[iImageReferenceIndex] && pTargetLayer->pImages[iImageRootIndex])
				pTargetLayer->pImages[iImageReferenceIndex] = NULL;
	
	// Release all of the images
	for(iImageRootIndex = 0; iImageRootIndex < pTargetLayer->cImageCount; iImageRootIndex++)
		ReleaseImage(pTargetLayer->pImages[iImageRootIndex]);
	
	// Free the images array, and the layer itself
	free(pTargetLayer->pImages);
	free(pTargetLayer);
	
}

void ReleaseScreen(Screen *pTargetScreen){
	int iLayerIndex;
	
	for(iLayerIndex = 0; iLayerIndex < pTargetScreen->cLayerCount; iLayerIndex++)
		ReleaseLayer(pTargetScreen->pLayers[iLayerIndex]);
	
	free(pTargetScreen->pLayers);
	free(pTargetScreen);
}

void ReleaseDisplay(Display *pTargetDisplay){
	int iScreenIndex;
	
	for(iScreenIndex = 0; iScreenIndex < pTargetDisplay->cScreenCount; iScreenIndex++)
		ReleaseScreen(pTargetDisplay->pScreens[iScreenIndex]);
	
	ReleaseImage(pTargetDisplay->pScreenLiteral);
	ReleaseLayer(pTargetDisplay->pSystemLayer);
	
	free(pTargetDisplay->pScreens);
	free(pTargetDisplay);
}

// Object Display Functions
/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
Image *UpdateImage(Image *pDisplayImage, Image *pImage, iPoint_2D *iLocationOffset){
	iPoint_2D iDestinationLocation;
	
	iDestinationLocation.X = iLocationOffset->X + pImage->pLocation.X;
	iDestinationLocation.Y = iLocationOffset->Y + pImage->pLocation.Y;
	
	memcpy_2D(pDisplayImage->pImageFormedData, pImage->pImageFormedData, &iDestinationLocation, &(pDisplayImage->pDimensions),
		&ZERO_2D, &(pImage->pDimensions), &(pImage->pDimensions));
	
	return pDisplayImage;
}

Image *UpdateLayer(Image *pDisplayImage, Layer *pLayer){
	// Find the visible Images and display them
	for(int iImageIndex = 0; iImageIndex < (int)pLayer->cImageCount; iImageIndex++)
		if(pLayer->pImages[iImageIndex]->bIsVisible)
			UpdateImage(pDisplayImage, pLayer->pImages[iImageIndex], &(pLayer->pLocation));
	
	return pDisplayImage;
}

/*Purpose: 
Arguments: *pDisplayImage - Pointer to the image that will be drawn to the screen.
	*pScreen - Pointer to the Screen that will be printed.
Returns: 
Bugs: None known.
*/
Image *UpdateScreen(Image *pDisplayImage, Screen *pScreen){
	// Find the visible Layers and display them
	for(int iLayerIndex = 0; iLayerIndex < (int)pScreen->cLayerCount; iLayerIndex++)
		if(pScreen->pLayers[iLayerIndex]->bIsVisible)
			UpdateLayer(pDisplayImage, pScreen->pLayers[iLayerIndex]);
	
	return pDisplayImage;
}

Display *UpdateDisplay(Display *pDisplay){
	// Clear the display image/object before writing to it.
	if(pDisplay->bClearOnUpdate)
		memset(pDisplay->pScreenLiteral->pImageFormedData, ' ', pDisplay->pScreenLiteral->iDisplaySize);

	
	// Find the visible screens and add them to the display image
	for(int iScreenIndex = 0; iScreenIndex < (int)pDisplay->cScreenCount; iScreenIndex++)
		if(pDisplay->pScreens[iScreenIndex]->bIsVisible){
			UpdateScreen(pDisplay->pScreenLiteral, pDisplay->pScreens[iScreenIndex]);
			break; // In ALL cases, we only show one screen at a time!
		}
	
	// Clear the screen. This is after image update, to reduce draw latency.
	if(pDisplay->bClearOnUpdate)
		ClearConsole();
	
	PrintImage(pDisplay->pScreenLiteral);
	
	return pDisplay;
}

/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
void PrintImage(Image *pImage){
	// Display the unmodified dest array at the cursor location.
	for(int iRowIndex = 0; iRowIndex < (int)pImage->pDimensions.Y; iRowIndex++){
			fprintf(stdout, "%.*s\n", pImage->pDimensions.X, (char *)(pImage->pImageFormedData + iRowIndex * sizeof(char) * pImage->pDimensions.X));
	}
}

// Memory manipulation
/*Purpose: Reorients a 1D array into a 2D block and places it into another
    separate larger or equal size 2D block at the provided location.
Arguments: *pDestination - The address of the memory cell to begin writing.
    *pSource - The address of the linear array to be converted and written.
    *iDestDimensions - The address of a iPoint_2D struct, with the 
        width (X) and height (Y) of the destination memory block.
    *iSourceDimensions - The address of a iPoint_2D struct, with the 
        width (X) and height (Y) of the reoriented linear array.
Returns: An address to where the data was copied (pDestination),
    or NULL for NULL inputs or failed writes.
Bugs: None known.
*/
void *memwrite_2D(void *pDestination, void *pSource, const iPoint_2D *iDestDimensions, const iPoint_2D *iSourceDimensions){
	int iRowIndex;
	
	if(!pDestination || !pSource || !iDestDimensions || !iSourceDimensions)
		return NULL;

	for(iRowIndex = 0; iRowIndex < iSourceDimensions->Y; iRowIndex++)
		if(!memcpy(pDestination + (iDestDimensions->X * iRowIndex), pSource + (iSourceDimensions->X * iRowIndex), iSourceDimensions->X))
			return NULL;

	return pDestination;
};

/*Purpose: Retrieves a 2D block and from another 2D block of equal or
    larger size and writes it to the linear array at the provided location.
Arguments: *pDestination - The address of the memory cell to begin writing.
    *pSource - The address of the memory cell to begin reading.
    *iDestDimensions - The address of a iPoint_2D struct, with the 
        width (X) and height (Y) of the block to be linearized.
    *iSourceDimensions - The address of a iPoint_2D struct, with the 
        width (X) and height (Y) of the larger/equal size block being read from.
Returns: An address to where the data was copied (pDestination),
    or NULL for NULL inputs or failed writes.
Bugs: None known.
*/
void *memread_2D(void *pDestination, void *pSource, const iPoint_2D *iDestDimensions, const iPoint_2D *iSourceDimensions){
	int iRowIndex;
	
	if(!pDestination || !pSource || !iDestDimensions || !iSourceDimensions)
		return NULL;

	for(iRowIndex = 0; iRowIndex < iDestDimensions->Y; iRowIndex++)
		if(!memcpy(pDestination + (iDestDimensions->X * iRowIndex), pSource + (iSourceDimensions->X * iRowIndex), iSourceDimensions->X))
			return NULL;

	return pDestination;
};

/*Purpose: Takes a 2D section from a specified location in a larger/equal 2D block
    and places it at a specified location in another larger/equal 2D block.
Arguments: *pDestination - A pointer to the memory block to be written to.
    *pSource - A pointer to the memory block the smaller/equal block is to be copied from.
    *iDestLocation - Pointer to a iPoint_2D with the X/Y coordinates of
        the first element to begin writing to as a 2D block.
    *iDestDimensions - A pointer to a iPoint_2D with the width and height
        of the destination block of memory.
    *iSourceLocation - Pointer to a iPoint_2D with the x/Y coordinates of
        the first element to be read as a 2D block.
    *iSourceDimensions - A pointer to a iPoint_2D with the width and height
         (X, Y respectively) of the source block of memory.
    *iCopyBlockDimensions - A pointer to a iPoint_2D with the width and height
        (X, Y respectively) of the block of memory to be copied from the source
        to the destination.
Returns: A pointer to the destination image, pDestination, or NULL for failed memory
    operations (malloc minimum equal to area of iCopyBlockDimensions),
    or NULL if any argument is NULL.
Bugs: The excessive number of arguments will increase likelyhood of errors. I'm trying
    to find a better interface, without putting additional work/computation on the programmer.
*/
void *memcpy_2D(void *pDestination, void *pSource, const iPoint_2D *iDestLocation, const iPoint_2D *iDestDimensions, const iPoint_2D *iSourceLocation, const iPoint_2D *iSourceDimensions, const iPoint_2D *iCopyBlockDimensions){
	void *pExtractedMemory;
	
	// If we are passed a NULL, return NULL
	if(!pDestination || !pSource || !iDestLocation || !iDestDimensions || !iSourceLocation || !iSourceDimensions || !iCopyBlockDimensions)
		return NULL;
	
	// Create a holding buffer for the memory slice.
	pExtractedMemory = malloc(iCopyBlockDimensions->X * iCopyBlockDimensions->Y);
	if(!pExtractedMemory || !pDestination || !pSource)// Verify allocation/pointers exist
		return NULL;
		
	// Use functions to extract 2D data as 1D, then 1D to new 2D location
	memread_2D(pExtractedMemory, pSource + (iSourceLocation->Y * iSourceDimensions->X) + iSourceLocation->X, iCopyBlockDimensions, iSourceDimensions);
	memwrite_2D((void *)(pDestination + (iDestLocation->Y * iDestDimensions->X) + iDestLocation->X), pExtractedMemory, iDestDimensions, iCopyBlockDimensions);
	
	free(pExtractedMemory);
	
	return pDestination;
}

// Manipulation and display
void ClearConsole(void){
	printf("\033[H\033[J");
}

void SetCursorLocation(iPoint_2D *TargetLocation){
	printf("\033[%d;%dH", TargetLocation->Y, TargetLocation->X);
}

int Clamp(const int iVariable, const int iMinimum, const int iMaximum){
	int iClampedValue = iVariable > iMaximum ? iMaximum : iVariable;
	iClampedValue = iClampedValue < iMinimum ? iMinimum : iClampedValue;
	
	if(iMinimum >= iMaximum)
		return 0;
	else
		return iClampedValue;
}

#endif
