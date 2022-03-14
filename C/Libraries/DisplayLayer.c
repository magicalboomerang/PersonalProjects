#define FILLCHARACTER 32

#include "ScreenString.h"
#include <unistd.h>

int main(){
	// Read the saved layer into memory as raw data.
	FILE *pLayerFile = fopen("TestLayer.gui", "r");
	void *pLayerData = malloc(82 + 18 * sizeof(int));
	fread(pLayerData, 1, 82 + 18 * sizeof(int), pLayerFile);
	fclose(pLayerFile);
	
	// Create a layer object based off of the file data.
	Layer *TestLayer = InitializeLayer(pLayerData);
	free(pLayerData);
	
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
	memset(LayerDisplay->pImageFormedData, FILLCHARACTER, LayerDisplay->iDisplaySize);
	
	printf("Print the blank screen.\n");
	PrintImage(LayerDisplay);
	//sleep(5);
	
	printf("Print the layer updated screen.\n");
	UpdateLayer(LayerDisplay, TestLayer);
	PrintImage(LayerDisplay);
	
	// Free the layer & image object
	ReleaseLayer(TestLayer);
	ReleaseImage(LayerDisplay);
	return 0;
}