#include <regx52.h>

void delayms(int);

sbit Moto = P0^0;
sbit moto_ = P0^1;
sbit moto1 = P2^1;
sbit moto2 = P2^2;

void main(){
	moto1 = 1;
	moto2 = 0;
	moto_ = 0;
	while(1){
		Moto = 1;
		delayms(10000);
		Moto = 0;
		delayms(10000);
	}
}

void delayms(int xms){
	unsigned int i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}