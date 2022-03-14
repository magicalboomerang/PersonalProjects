#include "ScreenString.c"

int main(){
	Image *Tester; 
	
	//iPoint_2D Dimensions = {.X = 10, .Y = 10};
	//int LinearArea = Dimensions.X * Dimensions.Y;
	char *sASCIIImage = "\010\000\000\000\010\000\000\000\010\000\000\000\010\000\000\000+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-";
	
	Tester = InitializeImage((void *)sASCIIImage);
	
	ClearConsole();
	
	PrintImage(Tester);
	
	printf("\n%s\n", *((char **)Tester));
	
	free(Tester);
	
	return 0;
}