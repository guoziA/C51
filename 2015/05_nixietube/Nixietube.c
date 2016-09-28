#include<regx52.h>

#define CONTROL P1//���ƶ˿�
#define SHOW P0//��ʾ�˿�

const long MAX = 100000;//���������ֵ
//const unsigned char number[] = {0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef};//����0~9,ʵ��
const unsigned char number[] = {0x77, 0x06, 0xb3, 0x97, 0xc6, 0xd5, 0xf5, 0x07, 0xf7, 0xd7};//��������0~9
void delayms(int xms){//��ʱ
	int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void showNumber(int position, int num){//��ʾ
	position = position%8;//��ֹposition����7
	num = num%10;//��ֹnum����9
	SHOW = 0x00;//�ÿ���ʾ
	CONTROL = ~(0x01 << position);//��position�������ͨ��
	SHOW = number[num];//��ʾnumber�����num��Ԫ��
}

void flash(int xms){//��˸
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
		number = MAX - time/11;//timeÿ��11����һ����
		if(number <= 0){//numberС�ڵ���0����ʼ��˸
			time = 0;
			flash(500);
		}
		for(position = 0; position < 8; position++){//ɨ���������ʾ����
			showNumber(position, number%10);
			number = number / 10;//�������λ������
		}
		time++;
	}
}