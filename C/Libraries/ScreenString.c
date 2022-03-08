#ifndef SCREENSTRING
#define SCREENSTRING

#include "ScreenString.h"

/* Plan of execution:
    1. 2d memory operations, and basic structs.
    2. Image base struct.
    3. Image creation functions
    4. Layer stuct + functions
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

// TEMPORARY FUNCTION
/*Purpose: 
Arguments: 
Returns: 
Bugs: None known.
*/
void printBlock(void *pMemoryBlock, iPoint_2D *BlockDimensions){
	// Display the unmodified dest array.
	for(int iRowIndex = 0; iRowIndex < BlockDimensions->X; iRowIndex++){
		for(int iColumnIndex = 0; iColumnIndex < BlockDimensions->Y; iColumnIndex++){
			printf("%c", ((char *)pMemoryBlock)[iRowIndex * BlockDimensions->X + iColumnIndex]);
		}
		printf("\n");
	}
}

#endif
