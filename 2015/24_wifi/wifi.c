#include<regx52.h>

void uartTimer2Init(){
	SCON = 0x50;//���ڷ�ʽ1���������
	TL2 = RCAP2L = 0xff;//�趨Ϊ��ʱ��2����115200������
	TH2 = RCAP2H = 0xfd;
	T2CON = 0x34;//RCLK=1��TCLK=1��TR2=1
	T2MOD = 0x00;
}