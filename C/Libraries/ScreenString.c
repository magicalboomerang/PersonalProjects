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
/*Purpose: This takes a FULLY FORMED AND FORMATTED Image binary object
	and places it into memory. Must be deallocated with ReleaseImage();
Arguments: *pImageFormedData - The binary data to be convereted into
	an image object.
Returns: An Image pointer to the newly created image.
Bugs: None known.
*/
Image *InitializeImage(void *pImageFormedData){
	Image *pInitializer;
	int pIntegerExtractor[4];
	
	if(!pImageFormedData){
		pInitializer = NULL;
	} else {
		// Note pIntegerExtractor realligns potential misalignments.
		memcpy((void *)pIntegerExtractor, pImageFormedData, 4 * sizeof(int));
		
		// Make space for the image object, and set up its details
		pInitializer = (Image *)malloc(sizeof(Image));
		pInitializer->ipLocation.X = pIntegerExtractor[0];
		pInitializer->ipLocation.Y = pIntegerExtractor[1];
		pInitializer->ipDimensions.X = pIntegerExtractor[2];
		pInitializer->ipDimensions.Y = pIntegerExtractor[3];
		pInitializer->iDisplaySize = pInitializer->ipDimensions.X * pInitializer->ipDimensions.Y;
		pInitializer->bIsVisible = 1; // All images are visible by default, for usability.
		
		// Make space for the image data. Separating the memory blocks ensures dynamic image sizes. The sizeof operator needs to be changed for non-ASCII application.
		pInitializer->pImageFormedData = malloc(sizeof(char) * pInitializer->iDisplaySize);
		
		memcpy(pInitializer->pImageFormedData, pImageFormedData + 4 * sizeof(int),sizeof(char) * pInitializer->iDisplaySize);
	}
	return pInitializer;
}

// NOTE: THE IMAGE BLOCK DOES NOT READ CHARACTERS FOR DIMENSIONAL DATA. IT TREATS THE
// DATA AS BINARY NOT CHARACTERS. This is intended to read data from file to initialize.
// 3/12 - Big changes, probably needs a refactor
Layer *InitializeLayer(void *sLayerFormedData){
	int *ipImage, iImageIndex;
	size_t iVariableReadOffset = 0;
	Layer *pInitializer;
	
	if(!sLayerFormedData){
		return NULL;
	} else {
		ipImage = (int *)sLayerFormedData;
		
		// Allocate the space for this Layer
		pInitializer = malloc(sizeof(Layer));
		
		// Extract the data from the ASCII block
		pInitializer->cImageCount = ipImage[0];
		pInitializer->cImageMaxCount = ipImage[1];
		pInitializer->ipLocation.X = ipImage[2];
		pInitializer->ipLocation.Y = ipImage[3];
		pInitializer->ipDimensions.X = ipImage[4];
		pInitializer->ipDimensions.Y = ipImage[5];
		pInitializer->iDisplaySize = pInitializer->ipDimensions.X * pInitializer->ipDimensions.Y;
		pInitializer->bIsVisible = 1;
		
		// Allocate the image address space. Separating the Layer and Image address lists in memory allows us to resize the maximum image count during run-time.
		pInitializer->pImages = (Image **)malloc(sizeof(Image) * pInitializer->cImageMaxCount);
		
		// Using running total offsets, properly read and write the images
		// I think there is a better solution to be found.
		for( iImageIndex = 0; iImageIndex < pInitializer->cImageCount; iImageIndex++){
			// Pass the address of the first byte of image data
			pInitializer->pImages[iImageIndex] = InitializeImage(sLayerFormedData + 6*sizeof(int) + iVariableReadOffset);
			
			// Calculate the new offsets. 4 ints + last image data
			iVariableReadOffset += 4*sizeof(int) + pInitializer->pImages[iImageIndex]->iDisplaySize;
		}
	}
	return (Layer *)pInitializer;
}

// Garbage collectors/object deallocation functions.
void ReleaseImage(Image *iTargetImage){
	if(iTargetImage->pImageFormedData) // If the pointer is not null...
		free(iTargetImage->pImageFormedData);// free the image data.
	free(iTargetImage);
}

void ReleaseLayer(Layer *lTargetLayer){
	int iImageRootIndex, iImageReferenceIndex;
	
	// If we have duplicate addresses, we must avoid double free calls
	// Iteratively look for duplicate addresses, if they are equal, set one to NULL.
	for(iImageRootIndex = 0; iImageRootIndex < lTargetLayer->cImageCount - 1; iImageRootIndex++)
		for(iImageReferenceIndex = iImageRootIndex + 1; iImageReferenceIndex < lTargetLayer->cImageCount; iImageReferenceIndex++)
			if(lTargetLayer->pImages[iImageRootIndex] == lTargetLayer->pImages[iImageReferenceIndex] && lTargetLayer->pImages[iImageRootIndex])
				lTargetLayer->pImages[iImageReferenceIndex] = NULL;
	
	// Release all of the images
	for(iImageRootIndex = 0; iImageRootIndex < lTargetLayer->cImageCount; iImageRootIndex++){
		ReleaseImage(lTargetLayer->pImages[iImageRootIndex]);
	}
	
	// Free the images array, and the layer itself
	free(lTargetLayer->pImages);
	free(lTargetLayer);
	
}

// Object Display Functions
/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
Image *UpdateImage(Image *pDisplayImage, Image *pImage, iPoint_2D *iLocationOffset){
	iPoint_2D iDestinationLocation;
	
	iDestinationLocation.X = iLocationOffset->X + pImage->ipLocation.X;
	iDestinationLocation.Y = iLocationOffset->Y + pImage->ipLocation.Y;
	
	memcpy_2D(pDisplayImage->pImageFormedData, pImage->pImageFormedData, &iDestinationLocation, &(pDisplayImage->ipDimensions),
		&ZERO_2D, &(pImage->ipDimensions), &(pImage->ipDimensions));
	
	return pDisplayImage;
}

Image *UpdateLayer(Image *pDisplayImage, Layer *pLayer){
	// Find the visible Images and display them
	for(int iImageIndex = 0; iImageIndex < (int)pLayer->cImageCount; iImageIndex++)
		if(pLayer->pImages[iImageIndex]->bIsVisible)
			UpdateImage(pDisplayImage, pLayer->pImages[iImageIndex], &(pLayer->ipLocation));
	
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
	// Find the visible screens and display them
	for(int iScreenIndex = 0; iScreenIndex < (int)pDisplay->cScreenCount; iScreenIndex++)
		if(pDisplay->pScreens[iScreenIndex]->bIsVisible)
			UpdateScreen((Image *)pDisplay, pDisplay->pScreens[iScreenIndex]);
	
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
	// Display the unmodified dest array at the cursor location.
	for(int iRowIndex = 0; iRowIndex < (int)pImage->ipDimensions.Y; iRowIndex++){
			fprintf(stdout, "%.*s\n", pImage->ipDimensions.X, (char *)(pImage->pImageFormedData + iRowIndex * sizeof(char) * pImage->ipDimensions.X));
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

#endif
