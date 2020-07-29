#include <stdio.h>

int main (void){

	unsigned int num = 0xAABBCCDD;
	
	printf("first number = %X\n", num);
	
	num = num & 0xFF00FFFF;
	num = num | 0xAAEECCDD;
	
	printf("final number = %X\n", num);

	return 0;
}
