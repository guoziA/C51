#include<regx52.h>

#define CONTROL P1 //数码管显示控制端口
#define SHOW P0 //数码管显示端口

const unsigned char leter[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //用00000001，00000010，,00000100，00001000，00010000，00100000，01000000，10000000检验数码管灯的位置

void delayms(int xms){
	int i = 0;
	int j = 0;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void showNum(unsigned int position)//第pos个数码管显示数字number
{
	
	position = position%8;//防止传入的参数position大于7
	SHOW = 0x00;//此时数码管不显示
	CONTROL = ~(0x01<<position);//将第position个数码管通电
	SHOW = ~leter[position];//显示leter数组第position个元素
	delayms(500);
}


void main()
{
	unsigned char position=0;
	while(1)
	{
		for(position=0; position<8; position++)//扫描显示8个数码管
		{
			showNum(position);//显示每个位置相应的字母
		}
	}
}