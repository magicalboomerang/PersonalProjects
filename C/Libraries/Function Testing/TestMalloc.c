#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	int *pTester = (int *)malloc(sizeof(int));
	
	memset((void *)pTester, 1, 1);
	
	printf("\n*pTester = 1: *pTester = %d\n", *pTester);
	
	int *pHolder = pTester;
	
	printf("pHolder = pTester -> 1: *pHolder = %d\n", *pHolder);
	
	memset((void *)pHolder, 2, 1);
	
	printf("*pHolder = 2: *pHolder = %d\npTester original address: %p\n", *pHolder, pTester);
	
	pTester = (int *)malloc(sizeof(int));
	
	printf("pTester new address: %p\n", pTester);
	
	memset((void *)pTester, 4, 1);
	
	printf("*pTester (new) = 4: *pTester = %d\n", *pTester);
	
	free(pTester);
	free(pHolder);
	
	return 0;
}
