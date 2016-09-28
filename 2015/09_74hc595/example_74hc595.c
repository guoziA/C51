#include<reg52.h> 
#include<intrins.h>
#define uchar unsigned char 
#define uint  unsigned int
#define CONTROL P1
void out();
sbit ru=P0^2;	   //595输入锁存，11脚
sbit shuju=P0^3;  //数据输入，14脚
sbit cu=P0^4;     //输出锁存，12脚
sbit OE_595 = P0^0;//高阻态，13脚

const unsigned char leter[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //?00000001,00000010,,00000100,00001000,00010000,00100000,01000000,10000000?????????

void delayms(int xms){//延时
	int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void delayus(int xus){
	int i, j;
	for(i = xus; i > 0; i--)
		for(j = 11; j > 0; j--);
}
void  sdata(uchar b1){   
	uchar c;
	for(c=0;c<8;c++)   //8位并行输出，从num中得到输出信息
	{    
		ru = 0;	  // 打开输入锁存,这时可以往Q0~Q7填入二进制数控制逻辑电位
		shuju=b1&0x80;	//清除数据的低7位，如10010011清除后为1，01001101清除后为0，然后填入Q0~Q7
		b1<<=1;		//右移如10010011右移后变为00100110，
		ru=1;				//关闭锁存，锁住一位
	}
	out();//输出到数码管
}
void out(void) {  
	cu=0;		 //打开输出锁存，取出Q0~Q7电位信息
	delayus(10); //延时，根据机器的频率决定时间
	cu=1;		// 关闭输出锁存，锁定Q0~Q7的电位信息，刷新屏幕
	delayus(10);  
} 
void main(){	
//	int i;
	OE_595 = 0;//开数码管显示
	while(1)	{		//-------------------------------数码管一个一个通电，相应数码管通电时显示相应的元素，快速切换使看起来是常亮状态--------------------------------------------------
//		for(i = 0; i < 8; i++){//循环输出leter字节组的8个元素
//			sdata(0xff);
//			CONTROL = (0x01 << i);
//			sdata(leter[i]);
//		}---------------------------------------------上面这样用8个数码管会闪烁，是不是频率太慢导致？-----------------------------------------------------------
		sdata(0xff);
		CONTROL = 0x01;
		sdata(leter[1]);
		delayms(30);
		sdata(0xff);
		CONTROL = 0x02;
		sdata(leter[2]);
		delayms(30);
	}
}
