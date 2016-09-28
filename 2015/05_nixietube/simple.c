#include<regx52.h>

#define CONTROL P1 //�������ʾ���ƶ˿�
#define SHOW P0 //�������ʾ�˿�

const unsigned char num[]={0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef}; //����0~9�͡�."��Ӧ����0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef

void showNum(unsigned int position, unsigned int number)//��pos���������ʾ����number
{
	
	position = position%8;//��ֹ����Ĳ���position����7
	number = number%10;//��ֹ����Ĳ���number����9
	SHOW = 0xff;//��ʱ����ܲ���ʾ
	CONTROL = ~(1<<position);//����position�������ͨ��
	SHOW = num[number];//P0״̬Ϊnumber��Ӧ����
}

void main()
{
	unsigned char position=0;
	unsigned long j=0;
	unsigned long m=0;
	while(1)
	{
		m = j/10;//jÿ����10��m����1
		for(position=0; position<8; position++)//ɨ����ʾ8�������
		{
			showNum(position,m%10);//��ʾ,��ȡ��λ��
			m = m/10;//ȡʮ���١��������������ɨ�裬��λȡ����Ӧ������
		}
		j++;//ÿ�仯һ������ɨ��ʮ��
	}
}