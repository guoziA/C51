#include <regx52.h>

sbit moto11 = P1^0;
sbit moto12 = P1^1;

void delayms(unsigned int xms);

void main(){
	moto11 = 0;
	moto12 = 0;
	while(1){
		moto12 = 0;
		moto11 = 1;
		delayms(3000);
		moto11 = 0;
		moto12 = 1;
		delayms(3000);
	}
}

void delayms(unsigned int xms){
	unsigned int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}