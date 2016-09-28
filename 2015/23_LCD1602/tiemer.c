#include <regx52.h>
#include <intrins.h>

#define LCD_Data P3   //数据端口
#define LCD_Busy P3_7 //忙检测位
#define LCD_RS   P1_0 //控制端口RS
#define LCD_RW   P1_1 //控制端口RW
#define LCD_E    P1_2 //控制端口E
#define LCD_BG   P1_5 //LCD背光控制
#define IO_TYPE P2_6  //type键IO口
#define IO_PLUS P2_5  //plus键IO口
#define IO_MINUS P2_4 //minus键IO口
#define IO_LOW  P2_0  //需拉低的IO口，用于上面三个键的检测
#define FREQ 12000000 //12M晶振
#define EVERY50ms (50000/(FREQ/12/1000000))//50ms需要的机器周期

code char *weekString[]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
unsigned char YEAR=13,MONTH=1,DAY=1,WEEK=1,HOUR=0,MIN=0,SEC=0;
unsigned char MONTH_LENGTH = 31;//当月的日期长度
enum setState{NO_SET,SET_HOUR,SET_MIN,SET_SEC,SET_YEAR,SET_MONTH,SET_DAY,SET_WEEK};//设置项枚举
enum setState setNow = NO_SET;//当前设置的项
enum keyVal{NO_KEY,TYPE_KEY,PLUS_KEY,MINUS_KEY};//键值枚举
enum keyVal keyNow = NO_KEY;//当前按下的键

void LCD_busy();//LCD忙检测
void LCD_WriteCode(unsigned char cmd);//向LCD发送命令cmd
void LCD_WriteData(unsigned char dat);//向LCD发送数据dat
void LCD_GoToXY(unsigned char x,unsigned char y);//将光标定位于(x,y)
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *str);//以(x,y)为起点写str指向的字符串
void LCD_Init();//LCD初始化
void LCD_Clear();//清屏
void secFlush();//当秒满60后更新相关信息
void minFlush();//当分满60后更新相关信息
void hourFlush();//当小时满24后更新相关信息
void dayFlush();//当日期满当月日期后更新相关信息
void monthLengthSet();//计算当月的天数
void monthFlush();//当月满12后更新相关信息
void timer2Init();//定时器2初始化程序, 其用于产生计时时钟
void timer0Init();//定时器0初始化，用于在LCD显示信息
enum keyVal getKey();//获取当前按键情况
void timeFlush();//用于设置时间时更新相关信息
void delay(unsigned int i);//延时


void main(){
	timer2Init();
	LCD_Init();
	timer0Init();
	LCD_BG = 0;//开启LCD背光
	while(1){
		keyNow = getKey();//获取当前按键情况
		if(NO_KEY == keyNow){//无键按下则继续while(1)
			continue;
		}else if(TYPE_KEY == keyNow){//设置类型键按下，则更新当前的设置项
			if(SET_WEEK == setNow){
				setNow = NO_SET;
			}else{
				setNow++;
			}
		}else if(NO_SET != setNow){//PLUS或MINUS键按下，并且当前的设置状态不为NO_SET
			timeFlush();
		}
		while(getKey() != NO_KEY);//等待按键松开
	}
}

void timeFlush(){
	if(PLUS_KEY == keyNow){
		switch(setNow){
			case SET_HOUR:
				HOUR= 23==HOUR? 0 : HOUR+1;
				break;
			case SET_MIN:
				MIN= 59==MIN? 0 : MIN+1;
				break;
			case SET_SEC:
				SEC= 59==SEC? 0 : SEC+1;
				break;
			case SET_YEAR:
				YEAR= 99==YEAR? 0 : YEAR+1;
				break;
			case SET_MONTH:
				MONTH= 12==MONTH? 1 : MONTH+1;
				monthLengthSet();//修改了月份，更新当月的天数
				DAY = MONTH_LENGTH<DAY? MONTH_LENGTH : DAY;
				break;
			case SET_DAY:
				DAY= MONTH_LENGTH==DAY? 1 : DAY+1;
				break;
			case SET_WEEK:
				WEEK= 6==WEEK? 0 : WEEK+1;
				break;
			default:
				break;
				}
	}else if(MINUS_KEY == keyNow){
		switch(setNow){
			case SET_HOUR:
				HOUR= 0==HOUR? 23 : HOUR-1;
				break;
			case SET_MIN:
				MIN= 0==MIN? 59 : MIN-1;
				break;
			case SET_SEC:
				SEC= 0==SEC? 59 : SEC-1;
				break;
			case SET_YEAR:
				YEAR= 0==YEAR? 99 : YEAR-1;
				break;
			case SET_MONTH:
				MONTH= 1==MONTH? 12 : MONTH-1;
				monthLengthSet();//修改了月份，更新当月的天数
				DAY = MONTH_LENGTH<DAY? MONTH_LENGTH : DAY;
				break;
			case SET_DAY:
				DAY= 1==DAY? MONTH_LENGTH : DAY-1;
				break;
			case SET_WEEK:
				WEEK= 0==WEEK? 6 : WEEK-1;
				break;
			default:
				break;
		}
	}
}
void timer0Init(){
	TMOD = 0x01;//定时器0工作模式1，16位定时器
	TL0 = (65536-EVERY50ms);//每50ms中断一次
	TH0 = (65536-EVERY50ms)>>8;
	ET0 = 1;//使能定时器0中断
	EA = 1;//使能总中断
	TR0 = 1;//定时器开始工作
}
void timer2Init(){
	T2CON = 0x00;//16位自动重装模式
	RCAP2H=TH2 = (65536-EVERY50ms)>>8;
	RCAP2L=TL2 = (65536-EVERY50ms);
	ET2 = 1;
	EA = 1;
	TR2 = 1;
}

void timer0() interrupt 1{//定时器0用于刷新显示
	static unsigned char tmp = 0;
	static unsigned char timeInf[9]="        ";//8个空格
	static unsigned char dateInf[11]="          ";//10个空格
	char *weekInf=weekString[WEEK];
	code unsigned char weekBli[4]="   ";//week设置闪烁内容
	TL0 = (65536-EVERY50ms);//重新赋初值
	TH0 = (65536-EVERY50ms)>>8;
	timeInf[0] = HOUR/10+'0';
	timeInf[1] = HOUR%10+'0';
	timeInf[2] = ':';
	timeInf[3] = MIN/10+'0';
	timeInf[4] = MIN%10+'0';
	timeInf[5] = ':';
	timeInf[6] = SEC/10+'0';
	timeInf[7] = SEC%10+'0';
	dateInf[0] = '2';
	dateInf[1] = '0';
	dateInf[2] = YEAR/10+'0';
	dateInf[3] = YEAR%10+'0';
	dateInf[4] = '-';
	dateInf[5] = MONTH/10+'0';
	dateInf[6] = MONTH%10+'0';
	dateInf[7] = '-';
	dateInf[8] = DAY/10+'0';
	dateInf[9] = DAY%10+'0';
	if(5>tmp){
		switch(setNow){
			case NO_SET:
				break;
			case SET_HOUR:
				timeInf[0]=timeInf[1]=' ';
				break;
			case SET_MIN:
				timeInf[3]=timeInf[4]=' ';
				break;
			case SET_SEC:
				timeInf[6]=timeInf[7]=' ';
				break;
			case SET_YEAR:
				dateInf[0]=dateInf[1]=dateInf[2]=dateInf[3]=' ';
				break;
			case SET_MONTH:
				dateInf[5]=dateInf[6]=' ';
				break;
			case SET_DAY:
				dateInf[8]=dateInf[9]=' ';
				break;
			case SET_WEEK:
				weekInf=weekBli;
				break;
			default:
				break;
		}
	}
	LCD_ShowString(4,0,timeInf);
	LCD_ShowString(0,1,dateInf);
	LCD_ShowString(13,1,weekInf);
	tmp++;
	if(10==tmp){
		tmp=0;
	}
}
void timer2() interrupt 5{//定时器2用于计时	
	static unsigned char every50ms = 0;
	TF2 = 0;//清零中断标志位
	every50ms++;
	if(20==every50ms){
		if(59==SEC){
			secFlush();
		}else{
			SEC++;
		}
		every50ms=0;
	}
}

void secFlush(){
	SEC = 0;
	if(59==MIN){
		minFlush();
	}else{
		MIN++;
	}
}
void minFlush(){
	MIN = 0;
	if(23==HOUR){
		hourFlush();
	}else{
		HOUR++;
	}
}
void hourFlush(){
	HOUR = 0;
	if(MONTH_LENGTH==DAY){
		dayFlush();
	}else{
		DAY++;
	}
	WEEK++;
	WEEK %= 7;
}
void dayFlush(){
	DAY = 1;
	if(12==MONTH){
		monthFlush();
	}else{
		MONTH++;
	}
	monthLengthSet();
}
void monthLengthSet(){
	switch(MONTH){
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			MONTH_LENGTH = 31;
			break;
		case 2:
			if(0==(YEAR%4)){
				MONTH_LENGTH = 29;
			}else{
				MONTH_LENGTH = 28;
			}
			break;
		default:
			MONTH_LENGTH = 30;
			break;
	}
}
void monthFlush(){
	MONTH=1;
	if(99==YEAR){//超过99后从0开始计数
		YEAR = 0;
	}else{
		YEAR++;
	}
}
void LCD_busy()	
{
   bit busy;
   LCD_E = 0;
   LCD_RS = 0;
   LCD_RW = 1;  
   do{
	  LCD_Busy = 1;
	  LCD_E = 1;
		_nop_(); 
	  busy = LCD_Busy;
	  LCD_E = 0;
	  }while(busy);
}
void LCD_WriteCode(unsigned char cmd)
{
   LCD_busy();
   LCD_E = 0;
   LCD_RS = 0;
   LCD_RW = 0;
   LCD_Data = cmd; 
   LCD_E = 1;
	 _nop_();
   LCD_E = 0;	
}
void LCD_GoToXY(unsigned char x,unsigned char y)
{
   if(y == 0){
	  LCD_WriteCode(0x80 | x);
   }else{
      LCD_WriteCode(0xC0 | x);
   }
}
void LCD_WriteData(unsigned char dat)
{
   LCD_busy();
   LCD_RS = 1;	  
   LCD_RW = 0;
   LCD_Data = dat; 
   LCD_E = 1;
	 _nop_();
   LCD_E = 0;
}
void LCD_Init()
{
   LCD_E = 0;
   LCD_WriteCode(0x08);//关显示 ，关光标显示  
   LCD_Clear();//清屏 
   LCD_WriteCode(0x38);//显示模式设定：16*2显示，5*7点阵，8位数据接口  
   LCD_WriteCode(0x06);//读或写一个字符后，地址加1，整屏不移动  
   LCD_WriteCode(0x0c);//开显示 

}
void LCD_Clear()
{
   LCD_WriteCode(0x01);
}
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *str){
	LCD_GoToXY(x,y);
	while(*str){
		LCD_WriteData(*str);
		str++;
	}
}
enum keyVal getKey(){
	IO_LOW = 0;
	if(!IO_TYPE){
		delay(1000);
		if(!IO_TYPE){
			return TYPE_KEY;
		}
	}else if(!IO_PLUS){
		delay(1000);
		if(!IO_PLUS){
			return PLUS_KEY;
		}
	}else if(!IO_MINUS){
		delay(1000);
		if(!IO_MINUS){
			return MINUS_KEY;
		}
	}
	return NO_KEY;
}
void delay(unsigned int i){
	for( ;i>0;i--);
}