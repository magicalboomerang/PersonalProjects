#include "matrix.h"

int main(){
	iMatrix TwobyTwo, TwobyThree, TwobyTen, ThreebyTwo, ThreebyThree, ThreebyTen, FiftyByFifty;//, TwoHundredByFifty;
	
	InitializeiMatrix( &TwobyTwo, 2, 2);
	InitializeiMatrix( &TwobyThree, 2, 3);
	InitializeiMatrix( &TwobyTen, 2, 10);
	InitializeiMatrix( &ThreebyTwo, 3, 2);
	InitializeiMatrix( &ThreebyThree, 3, 3);
	InitializeiMatrix( &ThreebyTen, 3, 10);
	InitializeiMatrix( &FiftyByFifty, 50, 50);
	//InitializeiMatrix( &TwoHundredByFifty, 200, 50);
	
	DisplayiMatrix(&TwobyTwo);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&TwobyThree);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&TwobyTen);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&ThreebyTwo);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&ThreebyThree);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&ThreebyTen);
	printf("\n-----------------------------------------------------------------------------------------\n");
	DisplayiMatrix(&FiftyByFifty);
	//printf("\n-----------------------------------------------------------------------------------------\n");
	//DisplayiMatrix(&TwoHundredByFifty);
	
	ReleaseiMatrix(&TwobyTwo);
	ReleaseiMatrix(&TwobyThree);
	ReleaseiMatrix(&TwobyTen);
	ReleaseiMatrix(&ThreebyTwo);
	ReleaseiMatrix(&ThreebyThree);
	ReleaseiMatrix(&ThreebyTen);
	ReleaseiMatrix(&FiftyByFifty);
	//ReleaseiMatrix(&TwoHundredByFifty);
	
	return 0;
}
