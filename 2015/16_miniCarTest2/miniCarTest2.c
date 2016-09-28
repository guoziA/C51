#include <regx52.h>


#define EVERY100US 100

void timer0Init();
void systemInit();
void driveMoto1(unsigned char);
void driveMoto2(unsigned char);
void trace();
void delay(unsigned char);
void MotosGo(unsigned char);

sbit senser1 = P1^0;
sbit senser2 = P1^1;
sbit light1 = P1^2;
sbit light2 = P1^3;
sbit moto1 = P1^4;
sbit moto2 = P1^5;

void main(){
	systemInit();
	timer0Init();
	while(1){
		if(!light1 && !light2){
			MotosGo(10);
		}
		if(!light1){
			driveMoto1(10);
			moto2 = 0;
		}																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						
		if(!light2){
			driveMoto2(10);
			moto1 = 0;
		}	
	}
}

void MotosGo(unsigned char dutyRatio){
	moto1 = 1;
	moto2 = 1;
	delay(dutyRatio);
	moto1 = 0;
	moto2 = 0;
	delay(10 - dutyRatio);
}
void driveMoto1(unsigned char dutyRatio){
	moto1 = 1;
	delay(dutyRatio);
	moto1 = 0;
	delay(10 - dutyRatio);
}

void driveMoto2(unsigned char dutyRatio){
	moto2 = 1;
	delay(dutyRatio);
	moto2 = 0;
	delay(10 - dutyRatio);
}

void systemInit(){
	senser1 = 0;
	senser2 = 0;
	light1 = 0;
	light2 = 0;
	moto1 = 0;
	moto2 = 0;
}

void timer0Init(){
	TMOD = 0X01;
	TH0 = (65536 - EVERY100US) / 256;
	TL0 = (65536 - EVERY100US) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void timer0() interrupt 1{
	TH0 = (65536 - EVERY100US) / 256;
	TL0 = (65536 - EVERY100US) % 256;
	trace();
}

void trace(){
	if(senser1){
		light1 = 1;
//		moto1 = 0;
	}else{
		light1 = 0;
	}
	if(senser2){
		light2 = 1;
//		moto2 = 0;
	}else{
		light2 = 0;
	}
}

void delay(unsigned char delayTime){
	unsigned char i,j;
	for(i = delayTime; i > 0; i--)
		for(j = 5; j > 0; j--);
}