#ifndef FEUSSEDEVTOOLS
#define FEUSSEDEVTOOLS

#include <stdio.h>
#include <stdlib.h>

void QuickBinary(int iConvertee){
	char *BINARYLITERAL[] = {"0000","0001","0010","0011","0100","0101","0110",\
		"0111","1000","1001","1010","1011","1100","1101","1110","1111"};
	int iNibbleValue, iNibbleBits; // lol
	
	for(iNibbleBits = 2 * sizeof(int); 0 < iNibbleBits; iNibbleBits--){
		iNibbleValue = (iConvertee >> (4 * (iNibbleBits - 1))) & 0x0000000F;
		printf("%s ", BINARYLITERAL[iNibbleValue]);
	}
}

#endif