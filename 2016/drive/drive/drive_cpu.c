#include "driveincludes.h"
/*
/********************************************************************************************************
/*											全局函数实现													*
/*																										*
/*	void drive_init();		初始化驱动																	*
/*	void delay_10us(uchar)	延时uchar个10us，不准确														*
/********************************************************************************************************
*/
void drive_init(){

#if HC_SR04_EN > 0

	HC_SR04_Init();
	
#endif
	
#if SERIAL_EN > 0
	
	SERIAL_timer2_Init();
	
#endif
	
#if DS18B20_EN > 0
	
	DS18B20_Init(100, -20, 0xff);
	
#endif
}

void delay_us(uchar xus){
	uchar i;
	for(i = xus; i > 0;i--);
}
