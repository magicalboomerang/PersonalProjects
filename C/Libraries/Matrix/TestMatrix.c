#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void TestSetM(iMatrix *pPointer, unsigned const char i, unsigned const char j, int k);
void TestGetM(iMatrix *pPointer, unsigned const char i, unsigned const char j);
void TestSetV(iMatrix *pPointer, unsigned const char i, int k);
void TestGetV(iMatrix *pPointer, unsigned const char i);

int main(int iArgumentCount, char *pArgumentList[]){
	int iCountIndex;
	iMatrix mTestMatrix, vTestVector;
	
	InitializeiMatrix(&mTestMatrix, 3, 3);
	
	DisplayiMatrix(&mTestMatrix);
	printf("\n\n");
	
	TestSetM(&mTestMatrix, 1, 1, 42);
	TestSetM(&mTestMatrix, 2, 2, ~0);
	TestGetM(&mTestMatrix, 2, 2);
	TestGetM(&mTestMatrix, 0, 0);
	TestSetM(&mTestMatrix, 0, 0, 17);
	TestGetM(&mTestMatrix, 0, 0);
	
	ReleaseiMatrix(&mTestMatrix);
	
	InitializeiVectorColumn(&vTestVector, 7);
	
	DisplayiMatrix(&vTestVector);
	printf("\n\n");
	
	TestSetV(&vTestVector, 3, -1000);
	TestSetV(&vTestVector, 0, 1);
	TestGetV(&vTestVector, 0);
	TestGetV(&vTestVector, 6);
	TestSetV(&vTestVector, 6, 1000);
	TestGetV(&vTestVector, 6);
	
	
	InitializeiMatrix(&mTestMatrix, 1, 7);
	
	SetiVectorElement(&mTestMatrix, 0, 1);
	
	for(iCountIndex = 0; iCountIndex < 7; iCountIndex++)
		SetiVectorElement(&vTestVector, iCountIndex, iCountIndex);
	
	DisplayiMatrix(&mTestMatrix);
	DisplayiMatrix(&vTestVector);
	
	for(iCountIndex = 1; iCountIndex < 7; iCountIndex++){
		SetiMatrixElement(&mTestMatrix, 0, iCountIndex - 1, 0);
		SetiMatrixElement(&mTestMatrix, 0, iCountIndex, 1);
		DisplayiMatrix(&mTestMatrix);
		printf("Linear Combination of test vectors: %d.\n", LinearCombinationiVector(&mTestMatrix, &vTestVector));
	}
	
	ReleaseiVector(&vTestVector);
	ReleaseiMatrix(&mTestMatrix);
	
	return 0;
}

// Quick test function, set a value then print the matrix.
void TestSetM(iMatrix *pPointer, unsigned const char i, unsigned const char j, int k){
	SetiMatrixElement(pPointer, i, j, k);
	
	DisplayiMatrix(pPointer);
	printf("\n\n");
}

// Quick test function for getting a value from a matrix
void TestGetM(iMatrix *pPointer, unsigned const char i, unsigned const char j){
	printf("Element %3d, %3d:\t%12d\n\n", i, j, GetiMatrixElement(pPointer, i, j));
}

// Quick test function, set a value then print the matrix.
void TestSetV(iMatrix *pPointer, unsigned const char i, int k){
	SetiVectorElement(pPointer, i, k);
	
	DisplayiMatrix(pPointer);
	printf("\n\n");
}

// Quick test function for getting a value from a matrix
void TestGetV(iMatrix *pPointer, unsigned const char i){
	printf("Element %3d:\t%12d\n\n", i, GetiVectorElement(pPointer, i));
}