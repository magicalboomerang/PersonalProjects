// !!! This could serve as the basis for '.gui' file generation !!!

#include "ScreenString.h"

int main(){
	unsigned int iDetailIndex;
	
	char *sImageBlocks[] = 
	{"111111222222333333",
	 "445566778899",
	 "0000000000000000000000000000000000000000000000000000"
	};
	
	int iLayerDetails[] = {3,10,1,1,100,50};
	
	iPoint_2D iImageDimensions[] = {{.X = 6, .Y = 3}, {.X = 2, .Y = 6}, {.X = 52, .Y = 1}};
	iPoint_2D iImageLocation[] = {{.X = 25, .Y = 57}, {.X = 99, .Y = 40}, {.X = 1, .Y = 1}};
	
	char *sLayerString = malloc(82 + 18 * sizeof(int));
	
	// Layer details
	// Actually easier to program with strcat, but this is already here
	for(iDetailIndex = 0; iDetailIndex < 6; iDetailIndex++){
		memcpy((void *)(sLayerString + sizeof(int) * iDetailIndex), (void *)(&iLayerDetails[iDetailIndex]), sizeof(int));
	}
	
	// set up a new basis
	void *pImageWrite = (void *)(sLayerString + 6*sizeof(int));
	
	for(iDetailIndex = 0; iDetailIndex < 3; iDetailIndex++){
		memcpy(pImageWrite, (void *)&(iImageLocation[iDetailIndex].X), sizeof(int));
		memcpy(pImageWrite + 1 * sizeof(int), (void *)&(iImageLocation[iDetailIndex].Y), sizeof(int));
		memcpy(pImageWrite + 2 * sizeof(int), (void *)&(iImageDimensions[iDetailIndex].X), sizeof(int));
		memcpy(pImageWrite + 3 * sizeof(int), (void *)&(iImageDimensions[iDetailIndex].Y), sizeof(int));
		memcpy(pImageWrite + 4 * sizeof(int), sImageBlocks[iDetailIndex], iImageDimensions[iDetailIndex].X * iImageDimensions[iDetailIndex].Y);
		pImageWrite += 4 * sizeof(int) + iImageDimensions[iDetailIndex].X * iImageDimensions[iDetailIndex].Y;
	}
	
	// Print the layer string data.
	for(iDetailIndex = 0; iDetailIndex < 154; iDetailIndex++){
		if(!(iDetailIndex % 4)){
			printf("\n------------  INT  -----------------\n");
		}
		printf("Character: %c\tHexadecimal: %.2X\n", sLayerString[iDetailIndex], (int)sLayerString[iDetailIndex]);
	}
	
	//printf("\nLiteral data: %s\n\n", sLayerString);
	printf("\n@@@@@@@@@@@@@@@@@@@@ Making Layer Object @@@@@@@@@@@\n");
	Layer *TestLayer = InitializeLayer((void *)sLayerString);
	printf("\n@@@@@@@@@@@@@@@@@@@@ Displaying Layer Object @@@@@@@@@@@\nLayer Address: %p", TestLayer);
	
	for(iDetailIndex = 0; iDetailIndex < (154 + sizeof(Layer)); iDetailIndex++){
		if(!(iDetailIndex % 4)){
			printf("\n------------  INT  -----------------\n");
		}
		printf("Character: %c\tHexadecimal: %.2X\n", ((unsigned char *)TestLayer)[iDetailIndex], ((unsigned char *)TestLayer)[iDetailIndex]);
	}
	
	ReleaseLayer(TestLayer);
	
	free(TestLayer);
	free(sLayerString);
	
	return 0;
}