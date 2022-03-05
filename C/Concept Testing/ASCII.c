#include <stdio.h>
#include <stdlib.h>

int main(){
	// Varspace
	
	
	// loop through all the characters!
	for(int index = 170; index < 226; index++){
		printf("\t%i %c%s",index, index, (index % 5) ? "" : "\n\n");
	}
	
	return 0;
}