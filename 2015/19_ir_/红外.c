#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
//������P3_3,(INT1)
sfr P4 = 0xE8;
sbit P4_6=P4^6;
void InitCom(void);
void ComOutChar(unsigned char OutData);
void DelayA(void);
void DelayB(void);
void Delayms(unsigned int xms);
void main(void)
{
	InitCom(); //��ʼ������
	EA = 1; //���� CPU �ж�
	IT1 = 1; //INT1 �½�����Ч
	EX1 = 1; //�� INT1 �ж�;

	while(1)	
	{

		P0_0 = 0; //����ָʾ LED
	}
}
//INT1 �ж�
unsigned char IRCode[2], IROK;
unsigned int TempCyc, TempCycB, TempCycA;
void INT1Fun(void) interrupt 1
{
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
		ComOutChar(IRCode[0]);
		ComOutChar(IRCode[1]); //���� PC ���ڲ鿴�Զ������������
		if (IRCode[1]==0x61) //1 �ż� //ֻ��ʾ���� 2 ֻ LED�����߿���������չ���Ʊ������
		P0_7 = 0;
		if (IRCode[1]==0xf1) //2 �ż�
		P0_6 = 0;

		for (TempCyc=0; TempCyc<300; TempCyc++)
		DelayA(); //��ʱ
	}
	EX1 = 1;
}
//�򴮿����һ���ַ������жϷ�ʽ��
void ComOutChar(unsigned char OutData)
{
	SBUF = OutData; //����ַ�
	while(!TI); //������ж��ַ��Ƿ���
	TI = 0; //�� TI
}
//���ڳ�ʼ�� ����Ϊ 12M ��ʽ 1 ������ 4800
void InitCom(void)
{
	SCON = 0x50; //���ڷ�ʽ 1,�������
	TMOD = 0x21; //��ʱ�� 1 ��ʱ��ʽ 2,��ʱ 0 Ϊģʽ 1��16 λģʽ
	TCON = 0x40; //�趨ʱ�� 1 ��ʼ����
	TH1 = 0xF3; //�貨����Ϊ 4800
	TL1 = 0xF3;
	PCON = 0x80; //�����ʼӱ�����,SMOD λ
	RI = 0; //���շ���־
	TI = 0;
	TR1 = 1; //������ʱ��
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