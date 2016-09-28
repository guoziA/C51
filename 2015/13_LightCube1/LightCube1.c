#include <regx52.h>
#include <intrins.h>
#include "constants.h";

void fall(unsigned char);
void delayms(int);
void delay01ms(int);
void cube(int);
void scan(char Data[],char frameNum,unsigned char n,unsigned int loop, char isNeedPuase,unsigned int breakTime);
void Timer0Init();
void lightInit();
void down_layer();
void spring(unsigned char n,unsigned int loop);
void rhythm(char a, unsigned char delayTime,unsigned int loop);
void showCode(unsigned char speed, unsigned int breakTime);
void alarm(unsigned char, unsigned int);
void rain();
void meteor(unsigned char speed);
void light_up(unsigned char);
void Caky(unsigned char speed, unsigned int breakTime);
void showHouse(unsigned char speed ,unsigned int breakTime);
void Tornado(unsigned char);
void Clock(unsigned char);
void Sin(unsigned char);
void happyBirthday(unsigned char times);
void happy();

unsigned char countPlus, isDownLayer = 0;
unsigned int count = 0;

void main(){
	Timer0Init();
	lightInit();
//	P1 = 0XFF;
//	P2 = 0XFF;
//	P0 = 0XFF;
	while(1){
		happy();
		
		Sin(3);
//		Clock(2);
//		Tornado(2);
//		showHouse(5,400);
//		Caky(5,400);
//		light_up(70);
//		happyBirthday(10);
//		scan(roll,28,2,100,0,1);
//		down_layer();
//		showCode(10,400);
//		rain();
///		spring(5,20);
//		cube(100);
//		fall(100);
	}
}

void happy(){
	scan(char_HAPPY,33,20,10,0,1);
	delayms(1000);
	scan(smile,1,1,1,1,200);
	lightInit();
	delayms(2000);
}

void happyBirthday(unsigned char times){
	rhythm(4,2 * times,1);
	rhythm(4,2 * times,1);
	rhythm(6,3 * times, 1);
	rhythm(6,5 * times, 1);
	rhythm(4,6 * times,1);
	rhythm(5,6 * times,1);
	
	delayms(700);
	rhythm(4,2 * times,1);
	rhythm(4,2 * times,1);
	rhythm(6,5 * times,1);
	rhythm(3,6 * times,1);
	rhythm(7,4 * times,1);
	rhythm(6,8 * times,1);
	
	delayms(700);
	rhythm(4,2 * times,1);
	rhythm(4,2 * times,1);
	rhythm(8,4 * times,1);
	rhythm(5,4 * times,1);
	rhythm(6,6 * times,1);
	rhythm(6,8 * times,1);
	
	delayms(700);
	rhythm(4,2 * times,1);
	rhythm(5,2 * times,1);
	rhythm(7,6 * times,1);
	rhythm(5,4 * times,1);
	rhythm(5,5 * times,1);
	rhythm(6,10 * times,1);

}
//显示数字
void showCode(unsigned char speed,unsigned int breakTime){
	alarm(3,500);
	scan(nuum_25,11,speed,1,1,breakTime);
	scan(nuum_1,11,speed,1,1,breakTime);
	scan(nuum_14,11,speed,1,1,breakTime);
	scan(nuum_18,11,speed,1,1,breakTime);
	scan(nuum_21,11,speed,1,1,breakTime);
	scan(nuum_25,11,speed,1,1,breakTime);
	scan(nuum_21,11,speed,1,1,breakTime);
	alarm(3,500);
}

void Sin(unsigned char speed){
	scan(sin1,15,speed, 1, 0,1);
	scan(sin2, 10, speed ,20, 0,1);
}

void Clock(unsigned char speed){
	scan(clock1, 8, speed, 1,0,1);
	scan(clock2, 14,speed, 50,0 ,1);
}

void Tornado(unsigned char speed){
	scan(tornado1,23,speed,1,0,1);
	scan(tornado2,14,speed,20,0,1);
	scan(tornado3,14,speed,20,0,1);
	scan(tornado4,23,speed,1,0,1);
}

void showHouse(unsigned char speed,unsigned int breakTime){
	scan(house1,8,speed,1,1,breakTime);
	scan(house2,8,speed,1,0,1);
}

void Caky(unsigned char speed, unsigned int breakTime){
	scan(cake, 9, speed,1,0,100);
	scan(cakeCandle,2,speed,1,1,breakTime);
}

void light_up(unsigned char speed){
	scan(lightUp1,8,speed,1,1,100);
	scan(lightUp2,8,speed,1,0,100);
}

//动画，雨
void rain(){
	char i;
	scan(line,33,5,1,0,1);
	P2 = 0XFF;
	P0 = 0XFF;
	for(i = 0; i < 8; i++){
		P1 = 0x01 << i;
		delayms(20);
	}
	lightInit();
	scan(drop,9,2,1,0,1);
	scan(rainDrop, 16, 2,100, 0,1);
}


//注意信号
void alarm(unsigned char n,unsigned int delayTime){
	char i = 0;
	for(i = 0; i < n; i++){
		P2 = 0XFF;
		P0 = 0XFF;
		P1 = 0XFF;
		delayms(delayTime);
		P1 = 0X00;
		delayms(delayTime);
	}
	lightInit();
}

//方块律动，边长，速度
void rhythm(char a,unsigned char delayTime,unsigned int loop){
	char i;
	unsigned int loop_k = 0;
	lightInit();
	for(loop_k = 0; loop_k < loop; loop_k++){
		for(i = 0; i < a; i++){
			P1 = ~(0X7F >> i);
			P2 = ~(0XFE << i);
			P0 = ~(0XFE << i);
			delayms(delayTime);
		}
		for(i = a-1; i > 0; i--){
			P1 = ~(0XFF >> i);
			P0 = 0X00;
			P2 = ~(0XFF << i);
			P0 = ~(0XFF << i);
			delayms(delayTime * 2);
		}
	}
}

//动画，弹簧
void spring(unsigned char n,unsigned int loop){
	char i = 0;
	unsigned loop_k = 0;
	lightInit();
	P1 = 0XFF;
	for(loop_k = 0; loop_k < loop; loop_k++){
		//横，向右，亮
		P2 = 0X00;
		P0 = 0XFF;
		for(i = 0; i < 9; i++){
			P2 = ~(0XFF << i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//纵，向后，灭
		P2 = 0XFF;
		P0 = 0XFF;
		for(i = 0; i < 8; i++){
			P0 = 0XFF << i;
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//纵，向前，亮
		P2 = 0XFF;
//		P0 = 0X00;
		for(i = 0; i < 9; i++){
			P0 = ~(0X7F >> i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//横，向右，灭
		P2 = 0X00;
		P0 = 0X00;
		for(i = 0; i < 8; i++){
			P2 = ~(0XFF << i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//横，向左，亮
		P2 = 0X00;
		P0 = 0XFF;
		for(i = 0; i < 9; i++){
			P2 = ~(0XFF >> i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//纵，向前灭
		P2 = 0XFF;
		P0 = 0XFF;
		for(i = 0; i < 8; i++){
			P0 = 0XFF >> i;
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//纵，向后，亮
		P2 = 0XFF;
//		P0 = 0X00;
		for(i = 0; i < 9; i++){
			P0 = ~(0XFE << i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		//横，向左，灭
		P2 = 0X00;
		P0 = 0X00;
		for(i = 0; i < 8; i++){
			P2 = ~(0XFF >>i);
			delayms((12 + 0.1 * (i * i - 7 * i - 30)) * n);
		}
		
	}
}
//扫描动画，参数：数据，帧数，每帧闪烁次数,是否暂停（1暂停，2不暂停，0循环），暂停时间
void scan(char Data[], char frameNum,unsigned char n,unsigned int loop, char isNeedPause, unsigned int breakTime){
	unsigned char i = 0,j=0, frame=0,times=0; 
	unsigned int k_loop = 0,loop_k = 0,stopTimes=0;
	for(k_loop = 0; k_loop < loop;){
//		for(loop_k = 0; loop_k < (frameNum+8) * 64; loop_k++){
//		while(1){
		if(frame >= frameNum) frame = 0;
		for(i = 0; i  < 8; i++){
			P1 = (0X01 << i);
			for(j = 0; j < 8; j++){
				P2 = (0x01 << j);
				P0 = Data[frame * 64 + i * 8 + j];
			}
			delay01ms(25);//25
		}
		//暂停后跳出循环
		if(frame == frameNum-1 ){
			k_loop++;
			if(isNeedPause == 1){
				k_loop = 0;
				times = n -1;
				frame--;
				stopTimes++;
			}
//			else if(isNeedPause == 2){//直接跳出循环
//				break;
//			}
		}
		times++;
		if(times % n == 0){
			times = 0;
			frame++;
		}
		if(stopTimes == breakTime){
			break;
		}
//		}
	}
}

//动画3，层层铺展
void down_layer(){
	char i,j ;
	isDownLayer = 1;
	P1 = 0X01;	
	P2 = 0Xff;
	P0 = 0X00;
	
	while(isDownLayer){
		for(j = 0; j < 8; j++){
			P2 = 0X00;
			P0 = 0XFF;
			for(i = 0; i < 9; i++){
				P2 = ~(0XFF << i);
				delayms(100);
			}	
			P2 = 0X00;
			P0 = 0X00;
			for(i = 0; i < 9; i++){
				P2 = ~(0XFF << i);
				delayms(100);
			}
			P1 = _crol_(P1,1);
		}
	}
}

//定时器
void time() interrupt 1{
	TL0 = (65536 - 50000) / 256;		
	TH0 = (65536 - 50000) % 256;		
	count++;
	
	
	if(count % 30 == 0){
		countPlus++;
		if(count >= 30000) count = 0;
	}
	if(countPlus % 10 == 0) isDownLayer = 0;
}

//动画2，矩形旋转
void cube(int times){
	int i;	
	P1 = 0XFF;
	for(i = 0; i < times; i++){
		delayms(500);
		P2 = 0XF0;
		P0 = 0X0F;
		P2 = 0X0F;
		P0 = 0XF0;

		delayms(500);

		P2 = 0XF0;
		P0 = 0XF0;
		P2 = 0X0F;
		P0 = 0X0F;
	}
}

//动画1，平行坠落
void fall(unsigned char times){
	unsigned char i; 
	P2 = 0xFF;
	P0 = 0Xff;
	P1 = 0X03;
	for(i = 0; i < times * 8; i++){
		delayms(50);
		P1 = _crol_(P1,1);
	}
	P0 = 0X00;
	P2 = 0X00;
}

//不精确延时1ms
void delayms(int xms){
	int i,j;
	for(i = xms; i > 0; i--)
		for(j = 950; j > 0; j--);
}
//不精确延时0.1ms，
void delay01ms(int x01ms){
	int i,j;
	for(i = x01ms; i > 0; i--)
		for(j = 95; j > 0; j--);
}

void Timer0Init(void)		
{
//	AUXR |= 0x80;		
	TMOD = 0x01;		
	TL0 = (65536 - 50000) / 256;		
	TH0 = (65536 - 50000) % 256;		
	EA = 1;
	ET0 = 1;		
	TR0 = 1;
}

//全部置位低电位
void lightInit(){
	P2 = 0XFF;
	P0 = 0X00;
	P2 = 0X00;
	P1 = 0X00;
}

void meteor(unsigned char speed){
	char i,j;
	lightInit();
	P2 =0X01;
	P0 = 0X01;
	for(i = 0; i < 8; i++){
		P2 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X80;
	P0 = 0X01;
	for(i = 1; i < 8; i++){
		P0 = 0X01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X80;
	P0 = 0X80;
	for(i = 1; i < 8; i++){
		P2 = 0X80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X01;
	P0 = 0X80;
	for(i = 1; i < 8; i++){
		P0 = 0x80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X01;
	P0 = 0X02;
	for(i = 1; i < 7; i++){
		P2 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X40;
	P0 = 0X02;
	for(i = 2; i < 7; i++){
		P0 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X40;
	P0 = 0X40;
	for(i = 2; i < 7; i++){
		P2 = 0x80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X02;
	P0 = 0X40;
	for(i = 2; i < 6; i++){
		P0 = 0x80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X02;
	P0 = 0X04;
	for(i = 2; i < 6; i++){
		P2 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X20;
	P0 = 0X04;
	for(i = 3; i < 6; i++){
		P0 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X20;
	P0 = 0X20;
	for(i = 3; i < 6; i++){
		P2 = 0x80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X02;
	P0 = 0X20;
	for(i = 3; i < 5; i++){
		P0 = 0x80 >> i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X02;
	P0 = 0X08;
	for(i = 3; i < 5; i++){
		P2 = 0x01 << i;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
	}
	lightInit();
	P2 = 0X10;
	P0 = 0X10;
	for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
		lightInit();
		P2 = 0X08;
		P0 = 0X10;
		for(j = 0; j < 8; j++){
			P1 = 0x01 << j;
			delayms(speed);
		}
		
}