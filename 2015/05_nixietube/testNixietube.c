#include<regx52.h>

#define CONTROL P1 //�������ʾ���ƶ˿�
#define SHOW P0 //�������ʾ�˿�

const unsigned char leter[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //��00000001��00000010��,00000100��00001000��00010000��00100000��01000000��10000000��������ܵƵ�λ��

void delayms(int xms){
	int i = 0;
	int j = 0;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void showNum(unsigned int position)//��pos���������ʾ����number
{
	
	position = position%8;//��ֹ����Ĳ���position����7
	SHOW = 0x00;//��ʱ����ܲ���ʾ
	CONTROL = ~(0x01<<position);//����position�������ͨ��
	SHOW = ~leter[position];//��ʾleter�����position��Ԫ��
	delayms(500);
}


void main()
{
	unsigned char position=0;
	while(1)
	{
		for(position=0; position<8; position++)//ɨ����ʾ8�������
		{
			showNum(position);//��ʾÿ��λ����Ӧ����ĸ
		}
	}
}