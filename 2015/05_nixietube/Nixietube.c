#include<regx52.h>

#define CONTROL P1//控制端口
#define SHOW P0//显示端口

const long MAX = 100000;//倒数的最大值
//const unsigned char number[] = {0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef};//数字0~9,实物
const unsigned char number[] = {0x77, 0x06, 0xb3, 0x97, 0xc6, 0xd5, 0xf5, 0x07, 0xf7, 0xd7};//仿真数字0~9
void delayms(int xms){//延时
	int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void showNumber(int position, int num){//显示
	position = position%8;//防止position大于7
	num = num%10;//防止num大于9
	SHOW = 0x00;//置空显示
	CONTROL = ~(0x01 << position);//第position个数码管通电
	SHOW = number[num];//显示number数组第num个元素
}

void flash(int xms){//闪烁
	while(1){
		CONTROL = 0xff;
		SHOW = 0xff;
		delayms(xms);
		SHOW = 0x00;
		CONTROL = 0x00;
		delayms(xms);
	}
}

void main(){
	unsigned int position = 0;
	long number = MAX;
	unsigned long time = 0;
	while(1){
		number = MAX - time/11;//time每过11倒数一个数
		if(number <= 0){//number小于等于0，开始闪烁
			time = 0;
			flash(500);
		}
		for(position = 0; position < 8; position++){//扫描数码管显示数字
			showNumber(position, number%10);
			number = number / 10;//提出高数位的数字
		}
		time++;
	}
}