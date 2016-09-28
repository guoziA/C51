#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
//������P3_3,(INT1)
#define IR P3_3
#define FRE 12000000//ʱ��Ƶ��12m
#define DIVI 12//��Ƶ
#define EVERY250US (250/(FRE/12/1000000))//50ms��������
sfr P4 = 0xE8;
sbit P4_6=P4^6;

void DelayA();
void DelayB();
void Delayms(unsigned int xms);
unsigned int Temp;

static int BEEP = 0;
int Counter = 0;




void main()
{
	EA = 1; //���� CPU �ж�
	IT1 = 1; //INT1 �½�����Ч
	EX1 = 1; //�� INT1 �ж�;
	IT0 = 1;
	EX0 = 1;
	TMOD = 0x02;//???0????2,8?????
	TH0=256-EVERY250US;//?TH0?TL0???,??250us???
	TL0=256-EVERY250US;
	ET0 = 1;//??T0??

	while(1){
		if(!P3_2){
			P0 = ~0X01;
			BEEP = 1;
			Delayms(5000);
			while(BEEP){
				P0 = 0X00 ;
			}
		}else{
			if(!BEEP)
				P0 = ~0X80;
		}
	}
}


/***************************�ⲿ�ж�0��������ʱ*****************************/


/***************************����***************************************/
//void timer() interrupt 1{
//	Counter++;
	
//}
/*
*******************�ⲿ�ж�1��ң��������*****************************
*/
//INT1 �ж�

void INT1Fun(void) interrupt 2
{
	unsigned char IRCode[2], IROK;
	unsigned int TempCyc, TempCycB, TempCycA;
	EX1 = 0; //�ⲿ�ж� 0 �ر�
	IROK = 0;
	while(P3_3){
		DelayB();
	}
	if (!P3_3) //����ǰ�벿�Ƿ���������������
	{
		DelayA();
		for (TempCycA=0; TempCycA<2; TempCycA++)
		{
			DelayA();
			if (P3_3) //����ǰ�벿�Ƿ���������������
			{
				for (TempCyc=0; TempCyc<300; TempCyc++)
					if (!P3_3) break; //�ȴ���һ��λ,
				if (TempCyc<300) //��ʱ����
				{
					for (TempCyc=0; TempCyc<8; TempCyc++)
					{
						while(!P3_3); //�ȴ� P3_2 ���ߣ���ʼλ���²���
						DelayB(); //����û�賬ʱ��ʵ��Ӧ���ڶ๦�ܵ����ʱӦ�賬ʱ
						IRCode[TempCycA] = IRCode[TempCycA]>>1;
						if (P3_3) //����ʱ 750us �� P3_2 ��Ϊ����ǰλΪ 1
						{
							IRCode[TempCycA] = IRCode[TempCycA] | 0x80;
							for (TempCycB=0; TempCycB<100; TempCycB++)
								if (!P3_3) break; //�ȴ��¸�λ ��λ 1 ʱ�ߵ�ƽΪ 1.5ms,
							if (TempCycB>99)//֮ǰ����ʱ�� 750us, ���Գ�ʱӦ���� 1.5ms-750us
								goto endchk; //�������Ϊ 1ms
						}
					}
				}
				else
					goto endchk; //��ʱ
			}	
			else
				goto endchk;
				IROK++; //���Զ���������붼���ʱΪ 2
		}	
	}
endchk:
	if (IROK==2)
	{
		if (IRCode[1]==0x61) {//1 �ż� //ֻ��ʾ���� 2 ֻ LED�����߿���������չ���Ʊ������
			BEEP = 0;
			P0 = ~0x80;
			TR0 = 0;
			IRCode[1] = 0x00;
		}
		if (IRCode[1]==0x71) //2 �ż�
		P0_6 = 0;

		for (TempCyc=0; TempCyc<300; TempCyc++)
		DelayA(); //��ʱ
	}
	EX1 = 1;
}

void DelayA()
{
	unsigned int I;
	for (I=0; I<800; I++); //3.5				650
}
void DelayB()
{
	unsigned int i;
	for (i=0; i<93; i++); // 0.75ms		93
}
void Delayms(unsigned int xms){
	unsigned int i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}