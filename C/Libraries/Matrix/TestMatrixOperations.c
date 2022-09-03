#include "matrix.h"

int main(){
	int ValueList[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6};
	
	iMatrix mIdentity, mCounting, mManipulator, vROnes, vRTwos, vRThrees, vCFours, vCFives, vCSixes, mWide, mTall, mWideCompute;
	
	CreateiMatrix(&mIdentity, 3, 3, &(ValueList[0]));
	CreateiMatrix(&mCounting, 3, 3, &(ValueList[9]));
	CreateiVectorRow(&vROnes, 3, &(ValueList[18]));
	CreateiVectorRow(&vRTwos, 3, &(ValueList[21]));
	CreateiVectorRow(&vRThrees, 3, &(ValueList[24]));
	CreateiVectorColumn(&vCFours, 3, &(ValueList[27]));
	CreateiVectorColumn(&vCFives, 3, &(ValueList[30]));
	CreateiVectorColumn(&vCSixes, 3, &(ValueList[33]));
	CreateiMatrix(&mWide, 3, 6, &(ValueList[18]));
	CreateiMatrix(&mTall, 6, 3, &(ValueList[18]));
	InitializeiMatrix(&mManipulator, 3, 3);
	InitializeiMatrix(&mWideCompute, 3, 6);
	
	DisplayiMatrix(&mIdentity);
	DisplayiMatrix(&mCounting);
	DisplayiMatrix(&vROnes);
	DisplayiMatrix(&vRTwos);
	DisplayiMatrix(&vRThrees);
	DisplayiMatrix(&vCFours);
	DisplayiMatrix(&vCFives);
	DisplayiMatrix(&vCSixes);
	DisplayiMatrix(&mWide);
	DisplayiMatrix(&mTall);
	DisplayiMatrix(&mManipulator);
	
	SetiMatrixRow(&mManipulator, 1, &vRTwos);
	DisplayiMatrix(&mManipulator);
	
	printf("Wide matrix error state: %d\n", (int)mWide._ERRORFLAG);
	SetiMatrixRow(&mWide, 1, &vRTwos);
	DisplayiMatrix(&mWide);
	printf("Wide matrix error state: %d\n", (int)mWide._ERRORFLAG);
	
	SetiMatrixColumn(&mManipulator, 2, &vCFours);
	DisplayiMatrix(&mManipulator);
	
	GetiMatrixRow(&vROnes, 0, &mIdentity);
	DisplayiMatrix(&vROnes);
	
	GetiMatrixColumn(&vCFours, 2, &mIdentity);
	DisplayiMatrix(&vCFours);
	
	printf("Get matrix error state: %d\n", (int)vROnes._ERRORFLAG);
	GetiMatrixRow(&vROnes, 1, &mWide);
	DisplayiMatrix(&vROnes);
	printf("Get matrix error state: %d\n", (int)vROnes._ERRORFLAG);
	
	printf("Get matrix error state: %d\n", (int)vROnes._ERRORFLAG);
	GetiMatrixRow(&vROnes, 1, &mTall);
	DisplayiMatrix(&vROnes);
	printf("Get matrix error state: %d\n", (int)vROnes._ERRORFLAG);
	
	mManipulator._ERRORFLAG = 0;
	DisplayiMatrix(&mManipulator);
	MultiplicationiMatrix(&mManipulator, &mIdentity, &mCounting);
	DisplayiMatrix(&mManipulator);
	printf("Get matrix error state: %d\n", (int)mManipulator._ERRORFLAG);
	
	mWide._ERRORFLAG = 0;
	printf("Get matrix error state: %d\n", (int)mWideCompute._ERRORFLAG);
	printf("Get matrix error state: %d\n", (int)mIdentity._ERRORFLAG);
	printf("Get matrix error state: %d\n", (int)mWide._ERRORFLAG);
	DisplayiMatrix(&mWideCompute);
	DisplayiMatrix(&mIdentity);
	DisplayiMatrix(&mWide);
	MultiplicationiMatrix(&mWideCompute, &mIdentity, &mWide);
	DisplayiMatrix(&mWideCompute);
	DisplayiMatrix(&mIdentity);
	DisplayiMatrix(&mWide);
	printf("Get matrix error state: %d\n", (int)mWideCompute._ERRORFLAG);
	printf("Get matrix error state: %d\n", (int)mIdentity._ERRORFLAG);
	printf("Get matrix error state: %d\n", (int)mWide._ERRORFLAG);
	
	ReleaseiMatrix(&mIdentity);
	ReleaseiMatrix(&mCounting);
	ReleaseiMatrix(&vROnes);
	ReleaseiMatrix(&vRTwos);
	ReleaseiMatrix(&vRThrees);
	ReleaseiMatrix(&vCFours);
	ReleaseiMatrix(&vCFives);
	ReleaseiMatrix(&vCSixes);
	ReleaseiMatrix(&mWide);
	ReleaseiMatrix(&mTall);
	ReleaseiMatrix(&mManipulator);
	
	return 0;
}
