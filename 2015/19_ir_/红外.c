#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
//红外收P3_3,(INT1)
sfr P4 = 0xE8;
sbit P4_6=P4^6;
void InitCom(void);
void ComOutChar(unsigned char OutData);
void DelayA(void);
void DelayB(void);
void Delayms(unsigned int xms);
void main(void)
{
	InitCom(); //初始化串口
	EA = 1; //允许 CPU 中断
	IT1 = 1; //INT1 下降沿有效
	EX1 = 1; //开 INT1 中断;

	while(1)	
	{

		P0_0 = 0; //工作指示 LED
	}
}
//INT1 中断
unsigned char IRCode[2], IROK;
unsigned int TempCyc, TempCycB, TempCycA;
void INT1Fun(void) interrupt 1
{
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
		ComOutChar(IRCode[0]);
		ComOutChar(IRCode[1]); //连接 PC 串口查看自定义码和数据码
		if (IRCode[1]==0x61) //1 号键 //只演示点亮 2 只 LED，读者可以自行扩展控制别的器件
		P0_7 = 0;
		if (IRCode[1]==0xf1) //2 号键
		P0_6 = 0;

		for (TempCyc=0; TempCyc<300; TempCyc++)
		DelayA(); //延时
	}
	EX1 = 1;
}
//向串口输出一个字符（非中断方式）
void ComOutChar(unsigned char OutData)
{
	SBUF = OutData; //输出字符
	while(!TI); //空语句判断字符是否发完
	TI = 0; //清 TI
}
//串口初始化 晶振为 12M 方式 1 波特率 4800
void InitCom(void)
{
	SCON = 0x50; //串口方式 1,允许接收
	TMOD = 0x21; //定时器 1 定时方式 2,定时 0 为模式 1，16 位模式
	TCON = 0x40; //设定时器 1 开始计数
	TH1 = 0xF3; //设波特率为 4800
	TL1 = 0xF3;
	PCON = 0x80; //波特率加倍控制,SMOD 位
	RI = 0; //清收发标志
	TI = 0;
	TR1 = 1; //启动定时器
}
void DelayA(void)
{
	unsigned int TempCyc;
	for (TempCyc=0; TempCyc<800; TempCyc++); //3.5				650
}
void DelayB(void)
{
	unsigned int TempCyc;
	for (TempCyc=0; TempCyc<93; TempCyc++); // 0.75ms
}
void Delayms(unsigned int xms){
	unsigned int i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}