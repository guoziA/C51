#include "driveincludes.h"
/*
/********************************************************************************************************
/*											ȫ�ֺ���ʵ��													*
/*																										*
/*	void drive_init();		��ʼ������																	*
/*	void delay_10us(uchar)	��ʱuchar��10us����׼ȷ														*
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
