#include<regx52.h>
#include "frams.h"


sbit SPEED = P3^1;
sbit MODE  = P3^0;

void InitINT0();
void SystemInit();

enum FLASH_SPEED{SLOWEST = 600, SLOWER = 500, NORMAL = 400, FAST = 300, FASTER = 200, FASTEST = 100
}flashSpeed;

enum FLASH_MODE{NUMBER, ANIMATION1, ANIMATION2 
}flashMode;

int speed;
void main(){
	SystemInit();
	InitINT0();
	while(1){
		if(flashMode == NUMBER){
			
		}else if(flashMode == ANIMATION1){
			
		}else if(flashMode == ANIMATION2){
			
		}
	}
}

//显示图案
//形状数组，帧数，加速或减速
void ShowFrame(char LS_code[], unsigned int frameNum, int delayFrameOffset){
	char graph;
	unsigned int i = 0, j = 0, k1 = 0, k2 = 0;
	//不允许speed<0
	if(delayFrameOffset >= flashSpeed){
		delayFrameOffset = flashSpeed - 20;
	}
	for(i = 0; i < frameNum;){
		if(k1 % 20 == 0){	//*************改变20可控制速度************
			k1 = 0;
			k2++;
			if(k2 >= speed){
				k2 = 0;
				i++;
				if(delayFrameOffset != 0){
					//使用变速
					speed = speed - delayFrameOffset;
				}else{
					//每帧之间检测速度
					speed = flashSpeed;
				}
			}
		}
		for(j = 0; j < 4; j++){
			graph = ((0xef << j) & 0xf0) + LS_code[i * 4 + j];
			P2 = graph;
		}
		k1++;
	}
}

void Break0() interrupt 0{
	if(MODE == 0){
		if(flashMode == ANIMATION2){
			flashMode = NUMBER;
		}else{
			flashMode++;
		}
	}
	else if(SPEED == 0){
		if(flashSpeed == FASTEST){
			flashSpeed = SLOWEST;
		}else{
			flashSpeed++;
		}
	}
}

void SystemInit(){
	flashSpeed = SLOWEST;
}

void InitINT0(){
	IT0 = 1;//外部中断0，下降沿触发（=0则为低电平触发）
	EX0 = 1;//使能外部中断0 enable
	IT1 = 0;
	EX1 = 1;

	EA = 1;//使总能中断 enable always
}