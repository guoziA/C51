#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
#define IR P3_3
#define FRE 12000000//ʱ��Ƶ��12m
#define DIVI 12//��Ƶ
#define EVERY250US (250/(FRE/12/1000000))//50ms��������

void DelayA();
void DelayB();
void Delayms(unsigned int xms);
unsigned int isBrk = 0;//����ֵ�����ж��Ƿ񱻴���
unsigned int Counter = 0;//����
int start;

void main()
{
	EA = 1; //���� CPU �ж�
	IT0 = 1;//�½����ж�
	EX0 = 1;//�ⲿ�ж�0����ң��
	IT1 = 1;//�ⲿ�ж�1���½����ж�
	EX1 = 1;//���ⲿ1�жϣ���������
	TMOD = 0x02;//��ʱ��0������ģʽ2
	TH0=256-EVERY250US;//250us���һ��
	TL0=256-EVERY250US;

	while(1){
		int i;
		for(i = 10; i--; i>0);
		if(isBrk){//�ж��Ƿ񱻴���״̬��
			P0 = ~0x01;//�����룬����
			if(Counter > 20000){//5s��û�н��������������Ϣ
				P0 = ~0x03;
			}
		}else{//�Ǵ���״̬����ң��
			if(Counter != 0){//��ʱ�������Ļ���ռ�ʱ���ؼ�ʱ��
				Counter = 0;
				TR0 = 0;
				ET0 = 0;
			}
			P0 = ~0x80;//��ȫ��ʾ
		}
	}
}


/**********************************j����*****************************************/

/***************************�ⲿ�ж�0��������ʱ*****************************/
void INT0Fun() interrupt 0{
	isBrk = 1;//������
	Counter = 0;//��ʱ����
	ET0 = 1;//����������ж�
	TR0 = 1;//��������
}

/***************************����***************************************/
void timer() interrupt 1{//�����������ڵ���5s
	Counter++;

}
/*
*******************�ⲿ�ж�1��ң��������*****************************
*/
//INT1 �ж�

void INT1Fun(void) interrupt 2
{
	if(!IR){//�а�������
		for(start = 0; start < 10; start++){//�ȴ�7.5ms
			DelayB();
			if(IR) return;//7.5msǰ��ɸߵ�ƽ������ң�����İ���
		}
		if(!IR){
			DelayB();//���ǵ�ƽ��������ʱ1.5ms
			if(IR) return;//�˹���P3_3��Ϊ�ߵ�ƽ����ң�����������˳�
			DelayB();
		}else{
			return;
		}
		if(IR){//��ʱ9ms�󣬽�������ĸߵ�ƽ
			for(start = 0; start < 4; start++){//��ʱ3ms
				DelayB();
				if(!IR) return;//�˹��̳��ֵ͵�λ����ң�����������˳�
			}
		}else{
			return;
		}
		if(IR){//��ʱ12ms���Ǹߵ�λ���ж���ң��������
			isBrk = 0;
		}else return;
	}else return;
}

void DelayA()
{//��ʱ3.5ms��û�õ�
	unsigned int I;
	for (I=0; I<800; I++); //3.5				650
}
void DelayB()
{//��ʱ0.75ms
	unsigned int i;
	for (i=0; i<90; i++); // 0.75ms		93
}
void Delayms(unsigned int xms){
	unsigned int i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}