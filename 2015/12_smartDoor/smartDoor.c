#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
//红外收P3_3,(INT1)
#define IR P3_3
#define FRE 12000000//时钟频率12m
#define DIVI 12//分频
#define EVERY250US (250/(FRE/12/1000000))//50ms机器周期
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
	EA = 1; //允许 CPU 中断
	IT1 = 1; //INT1 下降沿有效
	EX1 = 1; //开 INT1 中断;
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


/***************************外部中断0，触发计时*****************************/


/***************************警报***************************************/
//void timer() interrupt 1{
//	Counter++;
	
//}
/*
*******************外部中断1，遥控器控制*****************************
*/
//INT1 中断

void INT1Fun(void) interrupt 2
{
	unsigned char IRCode[2], IROK;
	unsigned int TempCyc, TempCycB, TempCycA;
	EX1 = 0; //外部中断 0 关闭
	IROK = 0;
	while(P3_3){
		DelayB();
	}
	if (!P3_3) //检验前半部是否过早结束，防干扰
	{
		DelayA();
		for (TempCycA=0; TempCycA<2; TempCycA++)
		{
			DelayA();
			if (P3_3) //检验前半部是否过早结束，防干扰
			{
				for (TempCyc=0; TempCyc<300; TempCyc++)
					if (!P3_3) break; //等待第一个位,
				if (TempCyc<300) //超时检验
				{
					for (TempCyc=0; TempCyc<8; TempCyc++)
					{
						while(!P3_3); //等待 P3_2 拉高，开始位的下部分
						DelayB(); //这里没设超时，实际应用在多功能的设计时应设超时
						IRCode[TempCycA] = IRCode[TempCycA]>>1;
						if (P3_3) //当延时 750us 后 P3_2 仍为高则当前位为 1
						{
							IRCode[TempCycA] = IRCode[TempCycA] | 0x80;
							for (TempCycB=0; TempCycB<100; TempCycB++)
								if (!P3_3) break; //等待下个位 当位 1 时高电平为 1.5ms,
							if (TempCycB>99)//之前已延时了 750us, 所以超时应大于 1.5ms-750us
								goto endchk; //这里最大为 1ms
						}
					}
				}
				else
					goto endchk; //超时
			}	
			else
				goto endchk;
				IROK++; //当自定码和数据码都完成时为 2
		}	
	}
endchk:
	if (IROK==2)
	{
		if (IRCode[1]==0x61) {//1 号键 //只演示点亮 2 只 LED，读者可以自行扩展控制别的器件
			BEEP = 0;
			P0 = ~0x80;
			TR0 = 0;
			IRCode[1] = 0x00;
		}
		if (IRCode[1]==0x71) //2 号键
		P0_6 = 0;

		for (TempCyc=0; TempCyc<300; TempCyc++)
		DelayA(); //延时
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