#include<regx52.h>

sbit moto1 = P2^0;

void delayms(unsigned int x);

void main(){
	moto1 = 0;
	moto_1 = 0;
	while(1){
		moto1 = 1;
		delayms(3000);
		moto1 = 0;
		delayms(3000);
	}
}

void delayms(unsigned int x){
	unsigned int i, j;
	for(i = x; i > 0; i--)
		for(j = 110; j > 0; j--);
}