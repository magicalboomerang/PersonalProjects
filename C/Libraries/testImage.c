#include "ScreenString.c"

int main(){
	Image *Tester; 
	
	iPoint_2D Dimensions = {.X = 10, .Y = 10};
	int LinearArea = Dimensions.X * Dimensions.Y;
	char *sASCIIImage = "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-";
	
	Tester = InitializeImage_String(malloc(LinearArea), sASCIIImage, &Dimensions, &ZERO_2D);
	
	ClearConsole();
	
	PrintImage(Tester);
	
	printf("\n%s\n", *((char **)Tester));
	
	free(Tester);
	
	return 0;
}