#include<regx52.h>

void uartTimer2Init(){
	SCON = 0x50;//串口方式1，允许接收
	TL2 = RCAP2L = 0xff;//设定为定时器2产生115200波特率
	TH2 = RCAP2H = 0xfd;
	T2CON = 0x34;//RCLK=1，TCLK=1，TR2=1
	T2MOD = 0x00;
}