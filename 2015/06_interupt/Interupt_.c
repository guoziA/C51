#include<regx52.h>

unsigned char i = 0;
unsigned char j = 0;

void main(){
	IT0 = 1;//外部中断0，下降沿触发（=0则为低电平触发）
	EX0 = 1;//使能外部中断0 enable
	IT1 = 0;
	EX1 = 1;

	EA = 1;//使总能中断 enable always

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