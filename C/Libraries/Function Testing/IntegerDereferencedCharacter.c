#include <stdio.h>
#include <stdlib.h>
#include "devtools.h"

int main(){
	char *sTester = "DEADBEEFTester String.";
	
	int *iHolder = (int *)sTester;
	
	printf("%X\n", *iHolder);
	printf("%s\n", (char *)iHolder);
	QuickBinary(*iHolder);
	printf("\n");
	
	return 0;
}