#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "devtools.h"

int main(int iArgumentCount, char *pArgumentList[]){
	int pIntReference = 20;
	char *pStringReference = (char *)&pIntReference;
	void *pTestBlankBlock = malloc(sizeof(int));
	
	printf("\n%d\n", pIntReference);
	printf("%p\n", &pIntReference);
	printf("%p\n", pStringReference);
	printf("%s\n", pStringReference);
	
	for(int iIndex = 0; iIndex < 4; iIndex++){
		QuickBinary((int)pStringReference[iIndex]);
		printf(" => %c\n", pStringReference[iIndex]);
	}
	
	// Copy as an int, display as int, then binary
	memcpy(pTestBlankBlock, &pIntReference, 4);
	printf("\nCopy as an int, display as int, then binary\nint: %d\n", pIntReference);
	for(int iIndex = 0; iIndex < 4; iIndex++){
		QuickBinary((int)pStringReference[iIndex]);
		printf(" => %c\n", pStringReference[iIndex]);
	}
	
	memset(pTestBlankBlock, 0, 4);
	
	// Copy as an int, display as int, then binary
	memcpy(pTestBlankBlock, pStringReference, 4);
	printf("\nCopy as an int, display as int, then binary\nint: %d\n", pIntReference);
	for(int iIndex = 0; iIndex < 4; iIndex++){
		QuickBinary((int)pStringReference[iIndex]);
		printf(" => %c\n", pStringReference[iIndex]);
	}
	
	memset(pTestBlankBlock, 0, 4);
	
	// Copy as an characters, display as int, then binary
	for(int iIndex = 0; iIndex < 4; iIndex++){
		memcpy(pTestBlankBlock, &pStringReference[iIndex], 1);
	}
	printf("\nCopy as a chars, display as int, then binary\nint: %d\n", pIntReference);
	for(int iIndex = 0; iIndex < 4; iIndex++){
		QuickBinary((int)pStringReference[iIndex]);
		printf(" => %c\n", pStringReference[iIndex]);
	}
	
	free(pTestBlankBlock);
	
	return 0;
}
