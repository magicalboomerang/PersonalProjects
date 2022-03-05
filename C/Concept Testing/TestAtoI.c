#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	char *TestElements[] = {"12345","DEADBEEF","0xDEADBEEF","0o76543"};
	int ConvertedValue = 0;
	
	
	for(int index = 0; index < 4; index++){
		ConvertedValue = atoi(TestElements[index]);
		printf("%s - %u - %x - %o\n", TestElements[index], ConvertedValue, ConvertedValue, ConvertedValue);
	}
	
	return 0;
}