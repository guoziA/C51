#include<regx52.h>


void UartTimer2Init()		//345600bps@11.0592MHz ��ʱ��2����������
{
	SCON = 0x50;//���ڷ�ʽ1���������
	TL2 = RCAP2L = 65535;//��߲�����
	TH2 = RCAP2H = 65535>>8;
	T2CON = 0x34;//ʹ��ʱ������������
	T2MOD = 0x00;
}

void uartTimer1Init(){//57600bps,11.0592MHZ��ʱ��1����������
	PCON = 0x80; //��λ�����ʣ��ӱ�����SMODλ
	SCON = 0x50; //���ڷ�ʽ1���������
	TMOD = 0x20; //��ʱ��1��ʽ2
	TH1 = 0xFF; //�������
	TL1 = 0xFF;
	TR1 = 1; //�򿪶�ʱ��
}

//��������
void sendChar(char dat){
	SBUF = dat;
	while(!TI);
	TI = 0;
}

//��������
char receiveMsg(){
	while(!RI);
	RI = 0;
	return SBUF;
}

//���жϷ�ʽ��������
void receive() interrupt 4{
	if(RI){
		RI = 0;
		P0 = SBUF;
	}
}


void main(){
	char inf[] = "�����Ƽ���Ϊ���������\n";
	char hello[] = "Hello World!\n";
	unsigned char i;
	unsigned char j=0;
//	EA = 1;//���ж� 
//	ES = 1;//�����ж�����
//	uartTimer1Init();//��ʱ��1����������
	UartTimer2Init();//��ʱ��2����������
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

