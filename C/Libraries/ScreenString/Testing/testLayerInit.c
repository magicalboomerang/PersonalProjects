// !!! This could serve as the basis for '.gui' file generation !!!

#include "ScreenString.h"

int main(){
	int iDetailIndex;
	
	char *sImageBlocks[] = 
	{"111111222222333333",
	 "445566778899",
	 "0000000000000000000000000000000000000000000000000000"
	};
	
	int iLayerDetails[] = {10,3,0,0,50,30};
	
	iPoint_2D iImageDimensions[] = {{.X = 6, .Y = 3}, {.X = 2, .Y = 6}, {.X = 51, .Y = 1}};
	iPoint_2D iImageLocation[] = {{.X = 43, .Y = 24}, {.X = 0, .Y = 0}, {.X = 30, .Y = 0}};
	
	char *sLayerString = malloc(82 + 18 * sizeof(int));
	
	// Layer details
	// Actually easier to program with strcat, but this is already here
	for(iDetailIndex = 0; iDetailIndex < 6; iDetailIndex++){
		memcpy((void *)(sLayerString + sizeof(int) * iDetailIndex), (void *)(&iLayerDetails[iDetailIndex]), sizeof(int));
	}
	
	// set up a new basis
	void *pImageWrite = (void *)&(sLayerString[6*sizeof(int)]);
	
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
	
	for(iDetailIndex = 0; iDetailIndex < (int)sizeof(Layer); iDetailIndex++){
		if(!(iDetailIndex % 4))
			printf("\n------------  INT  -----------------\n");
		printf("Character: %c\tHexadecimal: %.2X\n", ((unsigned char *)TestLayer)[iDetailIndex], ((unsigned char *)TestLayer)[iDetailIndex]);
	}
	
	
	// Need to test layer here to ensure it is actually being built correctly
	// Create a simple display image. This will be automated in the full process.
	Image *LayerDisplay = malloc(sizeof(Image));
		LayerDisplay->ipLocation.X = 0;
		LayerDisplay->ipLocation.Y = 0;
		LayerDisplay->ipDimensions.X = 100;
		LayerDisplay->ipDimensions.Y = 50;
		LayerDisplay->iDisplaySize = 5000;
		LayerDisplay->bIsVisible = 1;
	
	// Actually allocate the image and blank it.
	LayerDisplay->pImageFormedData = malloc(LayerDisplay->iDisplaySize * sizeof(char));
	memset(LayerDisplay->pImageFormedData, 0, LayerDisplay->iDisplaySize);
	
	PrintImage(LayerDisplay);
	//sleep(5);
	
	UpdateLayer(LayerDisplay, TestLayer);
	PrintImage(LayerDisplay);
	
	ReleaseImage(LayerDisplay);
	
	ReleaseLayer(TestLayer);
	
	// Write the Layer construction string to a file for later use
	FILE *output = fopen("TestLayer.gui", "w");
	
	fwrite(sLayerString, 1, 82 + 18 * sizeof(int), output);
	
	fclose(output);
	
	free(sLayerString);
	
	return 0;
}