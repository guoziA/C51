#include<regx52.h>

unsigned char i = 0;
unsigned char j = 0;

void main(){
	IT0 = 1;//�ⲿ�ж�0���½��ش�����=0��Ϊ�͵�ƽ������
	EX0 = 1;//ʹ���ⲿ�ж�0 enable
	IT1 = 0;
	EX1 = 1;

	EA = 1;//ʹ�����ж� enable always

	while(1);
}

void int0() interrupt 0{
	P0 = ~(0x01<<(i%8));
	i++;
}

void int1() interrupt 2{
	P1 = ~(0x80 >> ( j % 8));
	j++;
}