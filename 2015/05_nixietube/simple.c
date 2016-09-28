#include<regx52.h>

#define CONTROL P1 //数码管显示控制端口
#define SHOW P0 //数码管显示端口

const unsigned char num[]={0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef}; //数字0~9和“."对应的码0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef

void showNum(unsigned int position, unsigned int number)//第pos个数码管显示数字number
{
	
	position = position%8;//防止传入的参数position大于7
	number = number%10;//防止传入的参数number大于9
	SHOW = 0xff;//此时数码管不显示
	CONTROL = ~(1<<position);//将第position个数码管通电
	SHOW = num[number];//P0状态为number对应的码
}

void main()
{
	unsigned char position=0;
	unsigned long j=0;
	unsigned long m=0;
	while(1)
	{
		m = j/10;//j每增加10则m增加1
		for(position=0; position<8; position++)//扫描显示8个数码管
		{
			showNum(position,m%10);//显示,先取个位数
			m = m/10;//取十、百。。。数码管向左扫描，逐位取出相应的数字
		}
		j++;//每变化一个数字扫描十次
	}
}