#include <stdlib.h>
#include <stdio.h>
#include "ScreenString.h"

#define DISPLAYDETAILS 4
#define SCREENDETAILS 2
#define LAYERDETAILS 6

void WriteImage(FILE *pDestination, FILE *pSource, void *pHoldingSpace, int iImageCount);

int main(int iArgumentCount, char *pArgumentList[]){
	// Hard code the gui details
	int iDisplayDetails[DISPLAYDETAILS] = { 2, 2, 88, 30};
	int iSystemLayer[LAYERDETAILS] = {0, 0, 0, 0, 88, 30};
	int iSplashScreen[SCREENDETAILS] = {1, 1};
	int iPlayScreen[SCREENDETAILS] = {3, 3};
	int iSplashLayer[LAYERDETAILS] = {1, 1, 0, 0, 88, 30};
	int iBackgroundLayer[LAYERDETAILS] = { 3, 3, 0, 0, 88, 30};
	int iOperatingLayer[LAYERDETAILS] = { 5, 0, 0, 0, 88, 30};
	int iFeedbackLayer[LAYERDETAILS] = { 9, 9, 0, 0, 88, 30};
	
	void *pHoldingSpace = malloc(iDisplayDetails[2] * iDisplayDetails[3]);
	
	FILE *pOutputFile = fopen("Wordle.gui", "wb");
	
	// Splash screen
	fwrite(iDisplayDetails, sizeof(int), DISPLAYDETAILS, pOutputFile);
	fwrite(iSystemLayer, sizeof(int), LAYERDETAILS, pOutputFile);
	fwrite(iSplashScreen, sizeof(int), SCREENDETAILS, pOutputFile);
	fwrite(iSplashLayer, sizeof(int), LAYERDETAILS, pOutputFile);
	FILE *pSplashFile = fopen("SplashScreen.txt", "rb");
	WriteImage( pOutputFile, pSplashFile, pHoldingSpace, iSplashLayer[1]);
	fclose(pSplashFile);
	
	// Play Screen
	fwrite(iPlayScreen, sizeof(int), SCREENDETAILS, pOutputFile);
	fwrite(iBackgroundLayer, sizeof(int), LAYERDETAILS, pOutputFile);
	FILE *pBackgroundFile = fopen("BackgroundLayer.txt", "rb");
	WriteImage( pOutputFile, pBackgroundFile, pHoldingSpace, iBackgroundLayer[1]);
	fclose(pBackgroundFile);
	
	fwrite(iOperatingLayer, sizeof(int), LAYERDETAILS, pOutputFile);
	
	fwrite(iFeedbackLayer, sizeof(int), LAYERDETAILS, pOutputFile);
	FILE *pFeedbackFile = fopen("FeedbackLayer.txt", "rb");
	WriteImage( pOutputFile, pFeedbackFile, pHoldingSpace, iFeedbackLayer[1]);
	fclose(pFeedbackFile);
	
	fclose(pOutputFile);
	
	free(pHoldingSpace);
	
	return 0;
}


void WriteImage(FILE *pDestination, FILE *pSource, void *pHoldingSpace, int iImageCount){
	iPoint_2D pLocation, pDimensions;
	int iImageIndex;
	char sHolder[12];
	
	for(iImageIndex = 0; iImageIndex < iImageCount; iImageIndex++){
		fread( sHolder, sizeof(char), 11, pSource);
		sscanf(sHolder, "%d:%d;%d:%d", &(pLocation.X), &(pLocation.Y), &(pDimensions.X), &(pDimensions.Y));
		
		fwrite(&pLocation, sizeof(int), 2 ,pDestination);
		fwrite(&pDimensions, sizeof(int), 2 ,pDestination);
		
		fread(pHoldingSpace, sizeof(char), pDimensions.X * pDimensions.Y, pSource);
		fwrite(pHoldingSpace, sizeof(char), pDimensions.X * pDimensions.Y, pDestination);
	}
}