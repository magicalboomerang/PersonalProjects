/*

*/

#ifndef MATRIX
#define MATRIX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum MATERROR{NO_ERROR, ROW_MISMATCH, COLUMN_MISMATCH, DIMENSION_MISMATCH, ROW_OOB, COLUMN_OOB, NOT_VECTOR, NOT_MATRIX, ERROR_MAX};

struct T_IMATRIX{
	int *pElementList;
	char cROWS;
	char cCOLUMNS;
	
	// low level / memory manipulation variables
	size_t _MEMORYSIZE;
//	size_t _ROWSIZE;
	// error state tracker
	enum MATERROR _ERRORFLAG;
};


//----------------- Object Initialization and Destruction ----------------------------------------

/*Purpose: Fully initializes a zero matrix of the given dimensions.
Arguments: mEmptyMatrix - A pointer to an uninitialized matrix structure.
	cRowCount - a single unsigned byte with the number of matrix rows.
	cColumnCount - a single unsigned byte with the number of matrix columns.
Returns: A pointer to the initialized matrix (the same as mEmptyMatrix),
		OR NULL if the matrix cannot be allocated
Bugs: None known.
*/
iMatrix *InitializeiMatrix(iMatrix *mEmptyMatrix, const unsigned char cRowCount, const unsigned char cColumnCount){
	// Only initialize of the passed matrix exists, and has not already been allocated
	if(mEmptyMatrix){
		mEmptyMatrix->cROWS = cRowCount;
		mEmptyMatrix->cCOLUMNS = cColumnCount;
		mEmptyMatrix->_MEMORYSIZE = (size_t)(cRowCount * cColumnCount) * sizeof(int);
//		mEmptyMatrix->_ROWSIZE = ((size_t)cColumnCount) * sizeof(int);
		mEmptyMatrix->pElementList = (int *)malloc(mEmptyMatrix->_MEMORYSIZE);
		memset(mEmptyMatrix->pElementList, 0, mEmptyMatrix->_MEMORYSIZE);
		mEmptyMatrix->_ERRORFLAG = NO_ERROR;
	}
	
	// Return the allocated matrix, NULL, or an already existing passed matrix.
	return mEmptyMatrix;
}

iMatrix *CreateiMatrix(iMatrix *mEmptyMatrix, const unsigned char cRowCount, const unsigned char cColumnCount, int *ipElementList){
	InitializeiMatrix(mEmptyMatrix, cRowCount, cColumnCount);
	
	memcpy((void *)mEmptyMatrix->pElementList, (void *)ipElementList, (size_t)cRowCount * (size_t)cColumnCount * sizeof(int));
	
	return mEmptyMatrix;
}

iMatrix *InitializeiVectorColumn(iMatrix *mEmptyMatrix, const unsigned char cVectorLength){
	return InitializeiMatrix(mEmptyMatrix, cVectorLength, 1);
}

iMatrix *InitializeiVectorRow(iMatrix *mEmptyMatrix, const unsigned char cVectorLength){
	return InitializeiMatrix(mEmptyMatrix, 1, cVectorLength);
}

iMatrix *CreateiVectorColumn(iMatrix *mEmptyMatrix, const unsigned char cVectorLength, int *ipElementList){
	CreateiMatrix(mEmptyMatrix, cVectorLength, 1, ipElementList);
	
	return mEmptyMatrix;
}

iMatrix *CreateiVectorRow(iMatrix *mEmptyMatrix, const unsigned char cVectorLength, int *ipElementList){
	CreateiMatrix(mEmptyMatrix, 1, cVectorLength, ipElementList);
	
	return mEmptyMatrix;
}

/*Purpose: Releases the reserved memory associated with the matrix.
Arguments: mTargetMatrix - a pointer to the matrix in memory, to be freed.
Returns: None
Bugs: None known.
*/
void ReleaseiMatrix(iMatrix *mTargetMatrix){
	free(mTargetMatrix->pElementList);
}

/* This exists in case future vector memory implementation differs from matrixes
Purpose: Releases the reserved memory associated with the vector.
Arguments: mTargetVector - a pointer to the vector in memory, to be freed.
Returns: None
Bugs: None known.
*/
void ReleaseiVector(iMatrix *mTargetVector){
	ReleaseiMatrix(mTargetVector);
}


//-------------------------------------- Manipulation functions (Setters & Getters) -----------------
iMatrix *SetiMatrixElement(iMatrix *mSetMatrix, unsigned const char cZeroIndexRow, unsigned const char cZeroIndexColumn, int iElementValue){
	if(mSetMatrix->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexRow >= mSetMatrix->cROWS){
			mSetMatrix->_ERRORFLAG = ROW_OOB;
		} else if(cZeroIndexColumn >= mSetMatrix->cCOLUMNS){
			mSetMatrix->_ERRORFLAG = COLUMN_OOB;
		} else {
			memcpy(mSetMatrix->pElementList + (size_t)cZeroIndexColumn + (size_t)cZeroIndexRow * mSetMatrix->cCOLUMNS, &iElementValue, sizeof(int));
		}
	}
	return mSetMatrix;
}

iMatrix *SetiVectorElement(iMatrix *mSetVector, unsigned const char cZeroIndexElement, int iElementValue){
	if(mSetVector->_ERRORFLAG == NO_ERROR){
		// Using this function on a matrix will return the first element.
		if(mSetVector->cROWS != 1 && mSetVector->cCOLUMNS != 1){
			mSetVector->_ERRORFLAG = NOT_VECTOR;
		} else {
		// If it's a vector (one dimension = 1), handle it regardless of 'orientation'
		SetiMatrixElement(mSetVector,mSetVector->cCOLUMNS == 1 ? cZeroIndexElement : 0, mSetVector->cROWS == 1 ? cZeroIndexElement : 0, iElementValue);
		}
	}
	
	return mSetVector;
}

int GetiMatrixElement(iMatrix *mGetMatrix, unsigned const char cZeroIndexRow, unsigned const char cZeroIndexColumn){
	if(mGetMatrix->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexRow >= mGetMatrix->cROWS){
			mGetMatrix->_ERRORFLAG = ROW_OOB;
		} else if(cZeroIndexColumn >= mGetMatrix->cCOLUMNS){
			mGetMatrix->_ERRORFLAG = COLUMN_OOB;
		} else {
			return *(mGetMatrix->pElementList + (size_t)cZeroIndexRow * (size_t)mGetMatrix->cCOLUMNS + (size_t)cZeroIndexColumn);
		}
	}
	return 0;
}

int GetiVectorElement(iMatrix *mGetVector, unsigned const char cZeroIndexElement){
	if(mGetVector->_ERRORFLAG == NO_ERROR){
		// Using this function on a matrix will return the first element.
		if(mGetVector->cROWS != 1 && mGetVector->cCOLUMNS != 1){
			mGetVector->_ERRORFLAG = NOT_VECTOR;
		}
		// Handle vector regardless of 'orientation.'
		return GetiMatrixElement(mGetVector, mGetVector->cCOLUMNS == 1 ? cZeroIndexElement : 0, mGetVector->cROWS == 1 ? cZeroIndexElement : 0);
	}
	
	return 0;
}

// Set rows/columns
/*Purpose: Sets the row of a given matrix from a row vector.
Arguments: 
Returns: 
Bugs: By allowing the user to set from an int array, we weaken the encapsulation
	of the matrix object. It would be better to force matrix to matrix insertion,
	but this function needs to exist at some level.
*/
iMatrix *SetiMatrixRow(iMatrix *mSetMatrix, unsigned const char cZeroIndexRow, iMatrix *mReadMatrix){
	int iColumnIndex;
	// If the dimensions don't match, flag error, do nothing
	if(mSetMatrix->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexRow >= mSetMatrix->cROWS){
			mSetMatrix->_ERRORFLAG = ROW_OOB;
		} else if(mSetMatrix->cCOLUMNS != mReadMatrix->cCOLUMNS){
			mSetMatrix->_ERRORFLAG = COLUMN_MISMATCH;
		} else {
			// Loop through each element and copy
			for(iColumnIndex = 0; iColumnIndex < mSetMatrix->cCOLUMNS; iColumnIndex++)
				SetiMatrixElement(mSetMatrix, cZeroIndexRow, iColumnIndex, GetiVectorElement(mReadMatrix, iColumnIndex));
		}
	}
	
	return mSetMatrix;
}

iMatrix *SetiMatrixColumn(iMatrix *mSetMatrix, unsigned const char cZeroIndexColumn, iMatrix *mReadMatrix){
	int iRowIndex;
	// If the dimensions don't match, flag error, do nothing
	if(mSetMatrix->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexColumn >= mSetMatrix->cCOLUMNS){
			mSetMatrix->_ERRORFLAG = ROW_OOB;
		} else if(mSetMatrix->cROWS != mReadMatrix->cROWS){
			mSetMatrix->_ERRORFLAG = COLUMN_MISMATCH;
		} else {
			// Loop through each element and copy
			for(iRowIndex = 0; iRowIndex < mSetMatrix->cROWS; iRowIndex++)
				SetiMatrixElement(mSetMatrix, iRowIndex, cZeroIndexColumn, GetiVectorElement(mReadMatrix, iRowIndex));
		}
	}
	
	return mSetMatrix;
}

// Get rows/columns
iMatrix *GetiMatrixRow(iMatrix *mGetVectorRow, unsigned const char cZeroIndexRow, iMatrix *mReadMatrix){
	int iColumnIndex;
	
	if(mGetVectorRow->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexRow >= mReadMatrix->cROWS){
			mGetVectorRow->_ERRORFLAG = ROW_OOB;
		} else if(mGetVectorRow->cCOLUMNS != mReadMatrix->cCOLUMNS){
			mGetVectorRow->_ERRORFLAG = COLUMN_MISMATCH;
		} else {
			for(iColumnIndex = 0; iColumnIndex < mReadMatrix->cCOLUMNS; iColumnIndex++){
				SetiVectorElement(mGetVectorRow, iColumnIndex, GetiMatrixElement(mReadMatrix, cZeroIndexRow, iColumnIndex));
			}
		}
	}
	
	return mGetVectorRow;
}

iMatrix *GetiMatrixColumn(iMatrix *mGetVectorColumn, unsigned const char cZeroIndexColumn, iMatrix *mReadMatrix){
	int iRowIndex;
	
	if(mGetVectorColumn->_ERRORFLAG == NO_ERROR){
		if(cZeroIndexColumn >= mReadMatrix->cCOLUMNS){
			mGetVectorColumn->_ERRORFLAG = COLUMN_OOB;
		} else if(mGetVectorColumn->cROWS != mReadMatrix->cROWS){
			mGetVectorColumn->_ERRORFLAG = ROW_MISMATCH;
		} else {
			for(iRowIndex = 0; iRowIndex < mReadMatrix->cROWS; iRowIndex++){
				SetiVectorElement(mGetVectorColumn, iRowIndex, GetiMatrixElement(mReadMatrix, iRowIndex, cZeroIndexColumn));
			}
		}
	}
	
	return mGetVectorColumn;
}

//----------------- Mathematic Operations ----------------------------------------
int LinearCombinationiVector(iMatrix *vRowVector, iMatrix *vColumnVector){
	int iProductSum = 0, iElementIndex = 0;
	
	if(vRowVector->_ERRORFLAG == NO_ERROR && vColumnVector->_ERRORFLAG == NO_ERROR){
		if(vRowVector->cROWS != vColumnVector->cCOLUMNS){
			vRowVector->_ERRORFLAG = DIMENSION_MISMATCH;
			vColumnVector->_ERRORFLAG = DIMENSION_MISMATCH;
		}
		
		if(vRowVector->cROWS != 1 && vRowVector->cCOLUMNS != 1){
			vRowVector->_ERRORFLAG = NOT_VECTOR;
		}
		
		if(vColumnVector->cROWS != 1 && vColumnVector->cCOLUMNS != 1){
			vColumnVector->_ERRORFLAG = NOT_VECTOR;
		}
	}
	
	if(vRowVector->_ERRORFLAG == NO_ERROR && vColumnVector->_ERRORFLAG == NO_ERROR){
		for(iElementIndex = 0; iElementIndex < vColumnVector->cROWS; iElementIndex++){
			iProductSum += GetiVectorElement(vRowVector, iElementIndex) * GetiVectorElement(vColumnVector, iElementIndex);
		}
	}
	
	return iProductSum;
}

iMatrix *MultiplicationiMatrix(iMatrix *mOutputMatrix, iMatrix *mLeftMatrix, iMatrix *mRightMatrix){
	int iRowIndex, iColumnIndex;
	iMatrix vRowVector, vColumnVector;
	if(mOutputMatrix->_ERRORFLAG == NO_ERROR && mLeftMatrix->_ERRORFLAG == NO_ERROR && mRightMatrix->_ERRORFLAG == NO_ERROR){
		if(mLeftMatrix->cCOLUMNS != mRightMatrix->cROWS){
			mOutputMatrix->_ERRORFLAG = DIMENSION_MISMATCH;
		} else if(mOutputMatrix->cROWS != mLeftMatrix->cROWS){
			mOutputMatrix->_ERRORFLAG = ROW_MISMATCH;
		} else if(mOutputMatrix->cCOLUMNS != mRightMatrix->cCOLUMNS){
			mOutputMatrix->_ERRORFLAG = COLUMN_MISMATCH;
		}
		
		if(mOutputMatrix->_ERRORFLAG == NO_ERROR){
			// Initialize the temporary holding vectors
			InitializeiVectorRow(&vRowVector, mLeftMatrix->cCOLUMNS);
			InitializeiVectorColumn(&vColumnVector, mRightMatrix->cROWS);
			for(iRowIndex = 0; iRowIndex < mOutputMatrix->cROWS; iRowIndex++){
				GetiMatrixRow(&vRowVector, iRowIndex, mLeftMatrix);
				for(iColumnIndex = 0; iColumnIndex < mOutputMatrix->cCOLUMNS; iColumnIndex++){
					GetiMatrixColumn(&vColumnVector, iColumnIndex, mRightMatrix);
					SetiMatrixElement(mOutputMatrix, iRowIndex, iColumnIndex, LinearCombinationiVector(&vRowVector, &vColumnVector));
				}
			}
			// Free the temporary holding vectors
			ReleaseiVector(&vRowVector);
			ReleaseiVector(&vColumnVector);
		}
	}
	
	return mOutputMatrix;
}

//----------------- Output functions ----------------------------------------
iMatrix *DisplayiMatrix(iMatrix *mPrintMatrix){
	int iRowIndex, iColumnIndex;
	
	/*/ Print the matrix header
	printf("%c\t", 218);
	for(iColumnIndex = 0; iColumnIndex < (int)mPrintMatrix->cCOLUMNS; iColumnIndex++){
		printf("            \t");
	}
	printf("%c\n", 191);
	*/
	
	// Loop through all matrix element and print with a uniform width.
	for(iRowIndex = 0; iRowIndex < (int)mPrintMatrix->cROWS; iRowIndex++){
		printf("|\t");
		for(iColumnIndex = 0; (unsigned char)iColumnIndex < mPrintMatrix->cCOLUMNS; iColumnIndex++){
			printf("%12d\t", *((int *)(mPrintMatrix->pElementList + iRowIndex * (size_t)mPrintMatrix->cCOLUMNS/* mPrintMatrix->_ROWSIZE*/ + iColumnIndex)));
		}
		printf("|\n");
	}
	
	printf("\n");
	
	/*/ Print the matrix footer
	printf("%c\t", 192);
	for(iColumnIndex = 0; iColumnIndex < (int)mPrintMatrix->cCOLUMNS; iColumnIndex++){
		printf("            \t");
	}
	printf("%c\n", 217);
	*/
	return mPrintMatrix;
}

#endif