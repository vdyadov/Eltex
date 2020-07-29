#include <stdio.h>

int main(void){

	unsigned firstNum = 0xAABBCCDD;
	unsigned char finalNum = 0;
	int d = 0;

	for (int i =1; i < 5; ++i){
	
	finalNum = ((firstNum >> d) & 0x000000FF);
	printf("%d byte of firstNum = %X\n", i, finalNum);
	
	d += 8;
	
	}
	
	return 0;
}
