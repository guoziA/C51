#include <regx52.h>
#include <intrins.h>

#define LCD_Data P3   //���ݶ˿�
#define LCD_Busy P3_7 //æ���λ
#define LCD_RS   P1_0 //���ƶ˿�RS
#define LCD_RW   P1_1 //���ƶ˿�RW
#define LCD_E    P1_2 //���ƶ˿�E
#define LCD_BG   P1_5 //LCD�������
#define IO_TYPE P2_6  //type��IO��
#define IO_PLUS P2_5  //plus��IO��
#define IO_MINUS P2_4 //minus��IO��
#define IO_LOW  P2_0  //�����͵�IO�ڣ����������������ļ��
#define FREQ 12000000 //12M����
#define EVERY50ms (50000/(FREQ/12/1000000))//50ms��Ҫ�Ļ�������

code char *weekString[]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
unsigned char YEAR=13,MONTH=1,DAY=1,WEEK=1,HOUR=0,MIN=0,SEC=0;
unsigned char MONTH_LENGTH = 31;//���µ����ڳ���
enum setState{NO_SET,SET_HOUR,SET_MIN,SET_SEC,SET_YEAR,SET_MONTH,SET_DAY,SET_WEEK};//������ö��
enum setState setNow = NO_SET;//��ǰ���õ���
enum keyVal{NO_KEY,TYPE_KEY,PLUS_KEY,MINUS_KEY};//��ֵö��
enum keyVal keyNow = NO_KEY;//��ǰ���µļ�

void LCD_busy();//LCDæ���
void LCD_WriteCode(unsigned char cmd);//��LCD��������cmd
void LCD_WriteData(unsigned char dat);//��LCD��������dat
void LCD_GoToXY(unsigned char x,unsigned char y);//����궨λ��(x,y)
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *str);//��(x,y)Ϊ���дstrָ����ַ���
void LCD_Init();//LCD��ʼ��
void LCD_Clear();//����
void secFlush();//������60����������Ϣ
void minFlush();//������60����������Ϣ
void hourFlush();//��Сʱ��24����������Ϣ
void dayFlush();//���������������ں���������Ϣ
void monthLengthSet();//���㵱�µ�����
void monthFlush();//������12����������Ϣ
void timer2Init();//��ʱ��2��ʼ������, �����ڲ�����ʱʱ��
void timer0Init();//��ʱ��0��ʼ����������LCD��ʾ��Ϣ
enum keyVal getKey();//��ȡ��ǰ�������
void timeFlush();//��������ʱ��ʱ���������Ϣ
void delay(unsigned int i);//��ʱ


void main(){
	timer2Init();
	LCD_Init();
	timer0Init();
	LCD_BG = 0;//����LCD����
	while(1){
		keyNow = getKey();//��ȡ��ǰ�������
		if(NO_KEY == keyNow){//�޼����������while(1)
			continue;
		}else if(TYPE_KEY == keyNow){//�������ͼ����£�����µ�ǰ��������
			if(SET_WEEK == setNow){
				setNow = NO_SET;
			}else{
				setNow++;
			}
		}else if(NO_SET != setNow){//PLUS��MINUS�����£����ҵ�ǰ������״̬��ΪNO_SET
			timeFlush();
		}
		while(getKey() != NO_KEY);//�ȴ������ɿ�
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
				monthLengthSet();//�޸����·ݣ����µ��µ�����
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
				monthLengthSet();//�޸����·ݣ����µ��µ�����
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
	TMOD = 0x01;//��ʱ��0����ģʽ1��16λ��ʱ��
	TL0 = (65536-EVERY50ms);//ÿ50ms�ж�һ��
	TH0 = (65536-EVERY50ms)>>8;
	ET0 = 1;//ʹ�ܶ�ʱ��0�ж�
	EA = 1;//ʹ�����ж�
	TR0 = 1;//��ʱ����ʼ����
}
void timer2Init(){
	T2CON = 0x00;//16λ�Զ���װģʽ
	RCAP2H=TH2 = (65536-EVERY50ms)>>8;
	RCAP2L=TL2 = (65536-EVERY50ms);
	ET2 = 1;
	EA = 1;
	TR2 = 1;
}

void timer0() interrupt 1{//��ʱ��0����ˢ����ʾ
	static unsigned char tmp = 0;
	static unsigned char timeInf[9]="        ";//8���ո�
	static unsigned char dateInf[11]="          ";//10���ո�
	char *weekInf=weekString[WEEK];
	code unsigned char weekBli[4]="   ";//week������˸����
	TL0 = (65536-EVERY50ms);//���¸���ֵ
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
void timer2() interrupt 5{//��ʱ��2���ڼ�ʱ	
	static unsigned char every50ms = 0;
	TF2 = 0;//�����жϱ�־λ
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
	if(99==YEAR){//����99���0��ʼ����
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
   LCD_WriteCode(0x08);//����ʾ ���ع����ʾ  
   LCD_Clear();//���� 
   LCD_WriteCode(0x38);//��ʾģʽ�趨��16*2��ʾ��5*7����8λ���ݽӿ�  
   LCD_WriteCode(0x06);//����дһ���ַ��󣬵�ַ��1���������ƶ�  
   LCD_WriteCode(0x0c);//����ʾ 

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