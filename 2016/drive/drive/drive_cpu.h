#ifndef DRIVE_CPU_H
#define DRIVE_CPU_H

#include <regx52.h>
#include "drive_cfg.h"

/*
*********************************************************************************************************
*										ȫ�����Ƿ�����													*
*																										*
*																										*
*********************************************************************************************************
*/
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
/*
*********************************************************************************************************
*										��MCU�͵�·�йصı�������											*
*																										*
*																										*
*********************************************************************************************************
*/
/*********************************************����Ƶ��****************************************************/
#define SYS_FREQ	11059200u						


/*********************************************�������****************************************************/

#if	HC_SR04_EN > 0							/*HC_SR04ģ��												*/

sbit	HC_SR04_TRIG = P2^1;				/*TRIG														*/
sbit	HC_SR04_ECHO = P2^2;				/*ECHO														*/

#endif

#if	LED_EN > 0

#define LED_CONTROL	P1
#define LED_DATA	P0

#endif


#if	DS18B20_EN > 0

#define DS18B20_DQ	P3_7


#endif




/*
*********************************************************************************************************
*										���·�޹صĽӿں�������											*
*																										*
*																										*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*											ȫ�ֺ���														*
*																										*
*	void drive_init();		��ʼ������																	*
*	void delay_10us(uchar)	��ʱuchar��10us����׼ȷ														*
*********************************************************************************************************
*/

char* ItoC(uint);
void drive_init();


void delay_us(uchar xus);


/*
*********************************************************************************************************
*											�볬������й�												*
*																										*
*	float HC_SR04_getDistance()	���ز������� ��λcm														*
*	void HC_SR04_Init();		ģ���ʼ��																*
*	void HC_SR04_Trigger();		����																	*
*	float HC_SR04_Response()	������																	*
*********************************************************************************************************
*/
#if HC_SR04_EN > 0

extern uint HC_SR04_digit_dis;
void	HC_SR04_Init();
uint 	HC_SR04_GetDistance();

#endif



#if	SERIAL_EN > 0
extern char SERIAL_BUFFER;
char SERIAL_Receive();
void SERIAL_Print(char *dat);
void SERIAL_timer2_Init();
void SERIAL_Send_Char(char dat);

#endif


/*LED*/

#if LED_EN > 0

void LED_Display(ulong num);
void LED_Dispaly_Num(uchar num, uchar pos);

#endif


#if DS18B20_EN > 0 

#define	DS18B20_READ_ROM			0X33
#define DS18B20_MATCH_ROM			0X55
#define	DS18B20_SKIP_ROM			0XCC
#define	DS18B20_SEARCH_ROM			0XF0
#define	DS18B20_ALARM_SEARCH		0XEC
#define	DS18B20_WRITE_SCRACHPAD		0X4E
#define	DS18B20_READ_SCRACHPAD		0XBE
#define	DS18B20_COPY_SCRACHPAD		0X48
#define DS18B20_CONVERT_T			0X44
#define	DS18B20_RECALL_E2			0XB8
#define	DS18B20_READ_POWER_SUPPLY	0XB4

void DS18B20_Init(char th, char tl, char conf);
void DS18B20_ConvertT();
float DS18B20_GetT();

#endif

#endif