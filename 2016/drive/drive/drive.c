#include "driveincludes.h"

/*
*********************************************************************************************************
*											超声测距														*
*	接口函数：																							*
*	float HC_SR04_getDistance()	返回测量距离 单位cm														*
*	void HC_SR04_Trigger()		触发信号																	*
*	内部函数：																							*
*																										*
*		使能中断：																						*
*	void HC_SR04_INT_Init()		外部中断初始化															*
*	void HC_SR04_INT() interrupt 0	/void HC_SR04_INT() interrupt 2外中断响应函数						*
*	void HC_SR04_timer11_Init()	定时器1初始化															*
*	void HC_SR04_Init()			超声模块初始化															*
*	float HC_SR04_Response()	测距计算																	*
*																										*
*		不使能中断：																						*
*	void HC_SR04_getDigit_dis()	测量高电平时间长度														*
*	void HC_SR04_Init()			超声模块初始化															*
*	float HC_SR04_Response()	测量计算																	*
*																										*
*********************************************************************************************************
*/
#if	HC_SR04_EN > 0

uint HC_SR04_digit_dis;
uchar HC_SR04_Overflow;

	#if HC_SR04_TIMER_EN > 0
									
		void HC_SR04_timer1_Init(){					//初始化定时器1
			EA = 1;
			TMOD |= 0X10;							//定时器1中断方式1
			TH1 = 0;
			TL1 = 0;
			ET1 = 1;
			TR1 = 0;
		}
		
		void timer1_overflow() interrupt 3{
			HC_SR04_Overflow = 1;
		}
																//初始化	
		void HC_SR04_Init(){
			HC_SR04_ECHO = 0;
			HC_SR04_TRIG = 0;
			HC_SR04_Overflow = 0;
			HC_SR04_timer1_Init();
		}
		
		uint HC_SR04_GetDistance(){
			uint dis = 0;
			HC_SR04_TRIG = 1;
			delay_us(15);
			HC_SR04_TRIG = 0;
			while(!HC_SR04_ECHO);
			TR1 = 1;
			while(HC_SR04_ECHO);
			TR1 = 0;
			dis = TH1 * 256 + TL1;
			dis = dis * 17 / ((float)SYS_FREQ / 12000);
			if(dis > 200 || HC_SR04_Overflow){
				HC_SR04_Init();
				return 0;
			}
			TH1 = 0;
			TL1 = 0;
			return dis;
		}

	#else  
		void HC_SR04_getDigit_dis(){
			#pragma asm
				PUSH	DPH
				PUSH	DPL
				CLR		EA
				MOV		DPH,	#00H
				MOV		DPL,	#00H
				JNB		HC_SR04_ECHO,	$
			CTR_LOOP:
				INC		DPTR
				JB		HC_SR04_ECHO,	CTR_LOOP
				SETB	EA
				MOV 	HC_SR04_digit_dis,	DPH
				MOV		HC_SR04_digit_dis+01H,	DPL
				POP		DPL
				POP		DPH
				RET
			#pragma endasm
		}
		
		void HC_SR04_Init(){
			HC_SR04_ECHO = 0;
			HC_SR04_TRIG = 0;
			HC_SR04_digit_dis = 0;
		}
		
		void HC_SR04_Trigger(){
			HC_SR04_TRIG = 0;
			HC_SR04_TRIG = 1;
			delay_us(15);
			HC_SR04_TRIG = 0;
		}
		
		uint HC_SR04_Response(){
			uint distance = 0;	
			HC_SR04_getDigit_dis();
			distance = 4 * (1 + HC_SR04_digit_dis) * 17 / ((float)SYS_FREQ / 12000);
			HC_SR04_Init();
			return distance;
		}

		uint HC_SR04_GetDistance(){
			HC_SR04_Trigger();
			return HC_SR04_Response();
		}
	#endif
										

#endif
	
/*
*********************************************************************************************************
*											串口通信														*
*	接口函数：																							*
*	void SERIAL_Print(char *str)字符串为参数																*
*	char SERIAL_Receive()		接收，返回收到的数据														*
*	void SERIAL_Receive_INT() interrupt 4 使用中断接收，收到的数据在char SERIAL_BUFFER					*
*																										*
*	内部函数：																							*
*						*
*						*
*						*
*						*
*						*
*						*
*						*
*						*
*						*
*						*
*																										*
*********************************************************************************************************
*/
	
#if SERIAL_EN > 0

	char SERIAL_BUFFER;
	void SERIAL_timer2_Init(){
		uint temp;
		temp = (65536 - SYS_FREQ / (SERIAL_BAUD_RATE * 32));
		TH2 = RCAP2H = temp >> 8;
		TL2 = RCAP2L = temp;
		T2MOD = 0X00;
		T2CON = 0X34;
		SCON = 0X50;
	}
	
	char SERIAL_Receive(){
		while(!RI);
		RI = 0;
		return SBUF;
	}
	
	void SERIAL_Receive_INT() interrupt 4{
		if(RI){
			RI = 0;
			SERIAL_BUFFER = SBUF;
		}
	}
	
	void SERIAL_Send_Char(char dat){
		SBUF = dat;
		while(!TI);
		TI = 0;
	}
	
	void SERIAL_Print(char *dat){
		while(*dat){
			SERIAL_Send_Char(*dat);
			dat++;
		}
	}
	
#endif
	
	
#if LED_EN > 0
	
	const uchar LED_Num[] = {0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef};//0~9

	void LED_Display_Num(uchar num, uchar pos){
		num %= 10;
		pos %= 8;
		LED_CONTROL = 0XFF;
		LED_DATA = 0X00;
		LED_CONTROL = ~(0X01 << pos);
		LED_DATA = LED_Num[num];
	}
	
	void LED_Display(ulong num){
		uchar pos = 0, level = 0;
		ulong temp = num;
		for(; temp > 0; temp /= 10){
			level++;
		}
		for(; pos < level; pos++){
			LED_Display_Num(num % 10, pos);
			num /= 10;
		}
	}
	
#endif

	
#if	DS18B20_EN > 0
	
	bit DS18B20_Reset(){
		bit precence;
		uint ctr = 60000;
		DS18B20_DQ = 0;
		delay_us(90);
		DS18B20_DQ = 1;
		delay_us(11);
		while(DS18B20_DQ && ctr > 0) ctr--;
		precence = DS18B20_DQ;
		while(!DS18B20_DQ);
		return precence;
	}
	
	void DS18B20_Write(char dat){
		uchar i;
		for(i = 0; i < 8; i++){
			DS18B20_DQ = 0;
			delay_us(1);
			DS18B20_DQ = dat & 0x01;
			delay_us(8);
			DS18B20_DQ = 1;
			delay_us(1);
			dat >>= 1;
		}
	}
	
	void DS18B20_Config(char TempH, char TempL, char config){
		DS18B20_Write(DS18B20_WRITE_SCRACHPAD);
		DS18B20_Write(TempH);
		DS18B20_Write(TempL);
		DS18B20_Write(config);
	}
	
	void DS18B20_Init(char TempH, char TempL, char config){
		SERIAL_Print("ds18b20 init");
		if(DS18B20_Reset() == 0){
			DS18B20_Config(TempH, TempL, config);
			DS18B20_Reset();
			DS18B20_Write(DS18B20_SKIP_ROM);
			SERIAL_Print("config ok");
		}
	}
	
	void DS18B20_ConvertT(){
		DS18B20_Reset();
		DS18B20_Write(DS18B20_SKIP_ROM);
		DS18B20_Write(DS18B20_CONVERT_T);
	}
	
	uchar DS18B20_Read(){
		uchar i, temp;
		for(i = 0; i < 8; i++){
			temp >>= 1;
			DS18B20_DQ = 0;
			delay_us(1);
			DS18B20_DQ = 1;
			delay_us(6);
			if(DS18B20_DQ == 1){
				temp |= 0x80;
			}
			delay_us(8);
			DS18B20_DQ = 1;
			while(!DS18B20_DQ);
		}
		return temp;
	}
	
	int DS18B20_ReadT(){
		int temp;
		uchar temp1, temp2;
		DS18B20_Reset();
		DS18B20_Write(DS18B20_SKIP_ROM);
		DS18B20_Write(DS18B20_READ_SCRACHPAD);
		temp1 = DS18B20_Read();
		temp2 = DS18B20_Read();
		temp = (temp2 << 8) | temp1;
		SERIAL_Print("		");
		SERIAL_Print(ItoC(temp));
		return temp;
	}
	
	float DS18B20_GetT(){
		uchar decimal = 0;
		uchar isNegative = 0;
		int temp = DS18B20_ReadT();
		//SERIAL_Print("ds18b20 read temperature");
		if(temp >= 0){
			isNegative = 0;
		}else{
			isNegative = 1;
			temp = 0 - temp;
		}
		decimal = temp & 0x000f;
		temp >>= 4;
		temp = temp + (float)decimal / 16;
		//SERIAL_Print("	temperature: hex to dec:");
		//SERIAL_Print(ItoC(temp));
		if(isNegative){
			return -temp;
		}else{
			return temp;
		}
	}
	
#endif