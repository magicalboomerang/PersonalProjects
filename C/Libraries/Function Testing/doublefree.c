#include <stdio.h>
#include <stdlib.h>

int main(){
	void *pTester = malloc(sizeof(int) * 4);
	
	free(pTester);
	free(pTester);
}