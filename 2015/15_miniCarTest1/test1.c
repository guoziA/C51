#include <regx52.h>

sbit senser1 = P1^0;
sbit senser2 = P1^1;
sbit respond1 = P1^2;
sbit respond2 = P1^3;
sbit moto1 = P1^4;
sbit moto2 = P1^5;

void delayms(unsigned int );

void main(){
	senser1 = 0;
	senser2 = 0;
	respond1 = 1;
	respond2 = 1;
	while(1){
//		if(!senser1){
//			respond1 = 0;
//		}else{
//			respond1 = 1;
//		}
//		if(!senser2){
//			respond2 = 1;
//		}else{
//			respond2 = 0;
//		}
//		moto1 = 1;
		moto2 = 1;
		
		delayms(50000);
		moto1 = 0;
		moto2 = 0;
		delayms(50000);
	}
}

void delayms(unsigned int xms){
	unsigned char i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}