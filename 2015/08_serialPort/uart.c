#include<regx52.h>


void UartTimer2Init()		//345600bps@11.0592MHz 定时器2产生波特率
{
	SCON = 0x50;//串口方式1，允许接受
	TL2 = RCAP2L = 65535;//最高波特率
	TH2 = RCAP2H = 65535>>8;
	T2CON = 0x34;//使定时器产生波特率
	T2MOD = 0x00;
}

void uartTimer1Init(){//57600bps,11.0592MHZ定时器1产生波特率
	PCON = 0x80; //置位波特率，加倍控制SMOD位
	SCON = 0x50; //串口方式1，允许接受
	TMOD = 0x20; //定时器1方式2
	TH1 = 0xFF; //最大波特率
	TL1 = 0xFF;
	TR1 = 1; //打开定时器
}

//发送数据
void sendChar(char dat){
	SBUF = dat;
	while(!TI);
	TI = 0;
}

//接受数据
char receiveMsg(){
	while(!RI);
	RI = 0;
	return SBUF;
}

//用中断方式接受数据
void receive() interrupt 4{
	if(RI){
		RI = 0;
		P0 = SBUF;
	}
}


void main(){
	char inf[] = "启动科技，为爱好填动力！\n";
	char hello[] = "Hello World!\n";
	unsigned char i;
	unsigned char j=0;
//	EA = 1;//总中断 
//	ES = 1;//串口中断允许
//	uartTimer1Init();//定时器1产生波特率
	UartTimer2Init();//定时器2产生波特率
	while(1){
		for(i=0; i<24; i++){
			sendChar(inf[i]);
		}
		for(i=0; i<13; i++){
			sendChar(hello[i]);
		}
		sendChar(j+'0');
		j++;
		if(j>9){
			j = 0;
			break;
		}
		sendChar('\n');
		
//		P0 = receiveMsg();
	}
//	while(1);
}

