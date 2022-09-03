#ifndef MATRIX_H
#define MATRIX_H

enum MATERROR;
typedef struct T_IMATRIX iMatrix;

//----------------- Object Initialization and Destruction ----------------------------------------
iMatrix *InitializeiMatrix(iMatrix *mEmptyMatrix, const unsigned char cRowCount, const unsigned char cColumnCount);
iMatrix *CreateiMatrix(iMatrix *mEmptyMatrix, const unsigned char cRowCount, const unsigned char cColumnCount, int *ipElementList);
iMatrix *InitializeiVectorColumn(iMatrix *mEmptyMatrix, const unsigned char cVectorLength);
iMatrix *InitializeiVectorRow(iMatrix *mEmptyMatrix, const unsigned char cVectorLength);
iMatrix *CreateiVectorColumn(iMatrix *mEmptyMatrix, const unsigned char cVectorLength, int *ipElementList);
iMatrix *CreateiVectorRow(iMatrix *mEmptyMatrix, const unsigned char cVectorLength, int *ipElementList);

void ReleaseiMatrix(iMatrix *mTargetMatrix);
void ReleaseiVector(iMatrix *mTargetVector);

//-------------------------------------- Manipulation functions (Setters & Getters) -----------------
iMatrix *SetiMatrixElement(iMatrix *mSetMatrix, unsigned const char cZeroIndexRow, unsigned const char cZeroIndexColumn, int iElementValue);
iMatrix *SetiVectorElement(iMatrix *mSetVector, unsigned const char cZeroIndexElement, int iElementValue);
int GetiMatrixElement(iMatrix *mGetVector, unsigned const char cZeroIndexRow, unsigned const char cZeroIndexColumn);
int GetiVectorElement(iMatrix *mGetVector, unsigned const char cZeroIndexElement);

iMatrix *SetiMatrixRow(iMatrix *mSetMatrix, unsigned const char cZeroIndexRow, iMatrix *mReadMatrix);
iMatrix *SetiMatrixColumn(iMatrix *mSetMatrix, unsigned const char cZeroIndexRow, iMatrix *mReadMatrix);
iMatrix *GetiMatrixRow(iMatrix *mGetVectorRow, unsigned const char cZeroIndexRow, iMatrix *mReadMatrix);
iMatrix *GetiMatrixColumn(iMatrix *mGetVectorColumn, unsigned const char cZeroIndexColumn, iMatrix *mReadMatrix);

//----------------- Mathematic Operations ----------------------------------------
int LinearCombinationiVector(iMatrix *vRowVector, iMatrix *vColumnVector);

//----------------- Output functions ----------------------------------------
iMatrix *DisplayiMatrix(iMatrix *mPrintMatrix);

#include "matrix.c"

#endif