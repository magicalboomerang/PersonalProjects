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

/* Special development note: The memwrite/copy_2D functions are actually dimension lowering
functions. The 2D to linear actually takes an ND block and pulls a (N-1)D block.
Since all memory is in truth linear, we are simply transforming the representative dimensions
of a block. By the nature of how computers store memory, Taking a 2D slice from a 3D array is
equivalent to treating the 2D and 3D blocks as 1D and 2D blocks, respectively.
We can then make a recursive function to extract or modify blocks of any theoretic
dimension!

It is of special note, since multidimensional arrays ultimately are just uniformly spaced
memory elements, we can apply algebra to simplify the problem:
    Dn = Size of n-th dimension
    Xn = The selected element of the n-th dimension
 */

// Functions
// Display Structure initializers.
Image *InitializeImage_String(void *pAllocation, char *sImageASCII, const iPoint_2D *ipImageDimensions, const iPoint_2D *ipLayerLocation){
	Image *Initializer = pAllocation;
	
	if(!pAllocation || !sImageASCII || !ipImageDimensions || !ipLayerLocation){
		return NULL;
	} else {
		Initializer->sImageASCII = sImageASCII;
		Initializer->ipDimensions = *ipImageDimensions;
		Initializer->ipLocation = *ipLayerLocation;
		Initializer->iDisplaySize = ipImageDimensions->X * ipImageDimensions->Y;
		Initializer->bIsVisible = 1; // All images are visible by default, for usability.
	}
	return (Image *)pAllocation;
}

// NOTE: THE IMAGE BLOCK DOES NOT READ CHARACTERS FOR DIMENSIONAL DATA. IT TREATS THE
// DATA AS BINARY NOT CHARACTERS. This is intended to read data from file to initialize.
Layer *InitializeLayer_FormattedString(void *pImageAllocation, char *sFormattedLayerASCII){
	int *ipImage, iImageIndex;
	size_t iVariablReadOffset = 0, iVariableWriteOffset = 0;
	Layer *pInitializer = pImageAllocation;
	Image *pLastImage;
	iPoint_2D iImageLocation, iImageDimensions;
	
	if(!pImageAllocation || !sFormattedLayerASCII){
		return NULL;
	} else {
		ipImage = (int *)sFormattedLayerASCII;
		// Extract the data from the ASCII block
		pInitializer->cImageCount = ipImage[0];
		pInitializer->cImageMaxCount = ipImage[1];
		pInitializer->ipLocation.X = ipImage[2];
		pInitializer->ipLocation.Y = ipImage[3];
		pInitializer->ipDimensions.X = ipImage[4];
		pInitializer->ipDimensions.Y = ipImage[5];
		pInitializer->iDisplaySize = pInitializer->ipDimensions.X * pInitializer->ipDimensions.Y;
		pInitializer->bIsVisible = 1;
		
		// Set up the image address space.
		pInitializer->Images = pImageAllocation;
		
		// Using running total offsets, properly read and write the images
		// I think there is a better solution to be found.
		for( iImageIndex = 0; iImageIndex < pInitializer->cImageCount; iImageIndex){
			// Get 'image' details
			iImageLocation.X = *(ipImage + 6 + iVariablReadOffset);
			iImageLocation.X = *(ipImage + 7 + iVariablReadOffset);
			iImageDimensions.X = *(ipImage + 8 + iVariablReadOffset);
			iImageDimensions.X = *(ipImage + 9 + iVariablReadOffset);
			
			// Skip over the image just initialized to begin writing images
			pLastImage = InitializeImage_String((pInitializer + 1) + iVariableWriteOffset, ipImage + 10 + iVariablReadOffset, iImageDimensions, iImageLocation);
			
			// Calculate the new offsets.
			iVariablReadOffset += (size_t)(pLastImage->iDisplaySize + 4 * sizeof(int));
			iVariableWriteOffset += (size_t)(pLastImage->iDisplaySize + sizeof(Image));
		}
	}
	return (Layer *)pAllocation;
}
// Object Display Functions
/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
Image *UpdateImage(Image *pDisplayImage, Image *pImage, iPoint_2D *iLocationOffset){
	iPoint_2D iDestinationLocation, iZero;
	
	iDestinationLocation.X = iLocationOffset->X + pImage->ipDimensions.X;
	iDestinationLocation.Y = iLocationOffset->Y + pImage->ipDimensions.Y;
	
	iZero.X = 0;
	iZero.Y = 0;
	
	memcpy_2D(pDisplayImage, pImage, &iDestinationLocation, &(pDisplayImage->ipDimensions),
		&iZero, &(pImage->ipDimensions), &(pImage->ipDimensions));
	
	return pDisplayImage;
}

Image *UpdateLayer(Image *pDisplayImage, Layer *pLayer){
	// Find the visible Images and display them
	for(int iImageIndex = 0; iImageIndex < (int)pLayer->cImageCount; iImageIndex++)
		if(pLayer->pImages[iImageIndex].bIsVisible)
			UpdateImage(pDisplayImage, &pLayer->pImages[iImageIndex], &(pLayer->ipLocation));
	
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
		if(pScreen->pLayers[iLayerIndex].bIsVisible)
			UpdateLayer(pDisplayImage, &pScreen->pLayers[iLayerIndex]);
	
	return pDisplayImage;
}

Display *UpdateDisplay(Display *pDisplay){
	// Find the visible screens and display them
	for(int iScreenIndex = 0; iScreenIndex < (int)pDisplay->cScreenCount; iScreenIndex++)
		if(pDisplay->pScreens[iScreenIndex].bIsVisible)
			UpdateScreen((Image *)pDisplay, &pDisplay->pScreens[iScreenIndex]);
	
	// Clear the screen, Then print the generated screen.
	ClearConsole();
	PrintImage((Image *)pDisplay);
	
	return pDisplay;
}

/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
void PrintImage(Image *pImage){
	// Display the unmodified dest array.
	for(int iRowIndex = 0; iRowIndex < (int)pImage->ipDimensions.X; iRowIndex++){
		for(int iColumnIndex = 0; iColumnIndex < (int)pImage->ipDimensions.Y; iColumnIndex++){
			printf("%c", pImage->sImageASCII[iRowIndex * (int)(pImage->ipDimensions.X) + iColumnIndex]);
		}
		printf("\n");
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
void *memwrite_2D(void *pDestination, void *pSource, iPoint_2D *iDestDimensions, iPoint_2D *iSourceDimensions){
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
void *memread_2D(void *pDestination, void *pSource, iPoint_2D *iDestDimensions, iPoint_2D *iSourceDimensions){
	int iRowIndex;
	
	if(!pDestination || !pSource || !iDestDimensions || !iSourceDimensions)
		return NULL;

	for(iRowIndex = 0; iRowIndex < iDestDimensions->Y; iRowIndex++)
		if(!memcpy(pDestination + (iDestDimensions->X * iRowIndex), pSource + (iSourceDimensions->X * iRowIndex), iDestDimensions->X))
			return NULL;

	return pDestination;
};

/*Purpose: Takes a 2D section at a specified location from a larger/equal 2D block
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
void *memcpy_2D(void *pDestination, void *pSource, iPoint_2D *iDestLocation, iPoint_2D *iDestDimensions, iPoint_2D *iSourceLocation, iPoint_2D *iSourceDimensions, iPoint_2D *iCopyBlockDimensions){
	void *pExtractedMemory;
	
	// If we are passed a NULL, return NULL
	if(!pDestination || !pSource || !iDestLocation || !iDestDimensions || !iSourceLocation || !iSourceDimensions || !iCopyBlockDimensions)
		return NULL;

	
	// Create a holding buffer for the memory slice.
	pExtractedMemory = malloc(iCopyBlockDimensions->X * iCopyBlockDimensions->Y);
	if(!pExtractedMemory || !pDestination || !pSource) // Verify allocation/pointers exist
		return NULL;
	
	// Use functions to extract 2D data as 1D, then 1D to new 2D location
	memread_2D(pExtractedMemory, pSource + (iSourceLocation->Y * iSourceDimensions->X) + iSourceLocation->X, iCopyBlockDimensions, iSourceDimensions);
	memwrite_2D(pDestination + (iDestLocation->Y * iDestDimensions->X) + iDestLocation->X, pExtractedMemory, iDestDimensions, iCopyBlockDimensions);
	
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

#endif
