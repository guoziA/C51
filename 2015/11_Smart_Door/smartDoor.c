#include <AT89x52.h>
//http://weidu.baidu.com/view/c353e8360b4c2e3f57276349.html?re=view
#define IR P3_3
#define FRE 12000000//时钟频率12m
#define DIVI 12//分频
#define EVERY250US (250/(FRE/12/1000000))//50ms机器周期

void DelayA();
void DelayB();
void Delayms(unsigned int xms);
unsigned int isBrk = 0;//布尔值用来判断是否被闯入
unsigned int Counter = 0;//计数
int start;

void main()
{
	EA = 1; //允许 CPU 中断
	IT0 = 1;//下降沿中断
	EX0 = 1;//外部中断0开，遥控
	IT1 = 1;//外部中断1，下降沿中断
	EX1 = 1;//开外部1中断，触发报警
	TMOD = 0x02;//定时器0，工作模式2
	TH0=256-EVERY250US;//250us溢出一次
	TL0=256-EVERY250US;

	while(1){
		int i;
		for(i = 10; i--; i>0);
		if(isBrk){//判断是否被闯入状态，
			P0 = ~0x01;//被闯入，报警
			if(Counter > 20000){//5s后没有解除警报，发送信息
				P0 = ~0x03;
			}
		}else{//非闯入状态或按下遥控
			if(Counter != 0){//计时已启动的话清空计时，关计时器
				Counter = 0;
				TR0 = 0;
				ET0 = 0;
			}
			P0 = ~0x80;//安全提示
		}
	}
}


/**********************************j剪辑*****************************************/

/***************************外部中断0，触发计时*****************************/
void INT0Fun() interrupt 0{
	isBrk = 1;//被闯入
	Counter = 0;//计时清零
	ET0 = 1;//允许计数器中断
	TR0 = 1;//开计数器
}

/***************************警报***************************************/
void timer() interrupt 1{//计数器，用于倒数5s
	Counter++;

}
/*
*******************外部中断1，遥控器控制*****************************
*/
//INT1 中断

void INT1Fun(void) interrupt 2
{
	if(!IR){//有按键按下
		for(start = 0; start < 10; start++){//等待7.5ms
			DelayB();
			if(IR) return;//7.5ms前变成高电平，不是遥控器的按键
		}
		if(!IR){
			DelayB();//还是电平，则再延时1.5ms
			if(IR) return;//此过程P3_3变为高电平则不是遥控器按键，退出
			DelayB();
		}else{
			return;
		}
		if(IR){//延时9ms后，进入结果码的高电平
			for(start = 0; start < 4; start++){//延时3ms
				DelayB();
				if(!IR) return;//此过程出现低电位则不是遥控器按键，退出
			}
		}else{
			return;
		}
		if(IR){//延时12ms后是高电位，判断是遥控器按键
			isBrk = 0;
		}else return;
	}else return;
}

void DelayA()
{//延时3.5ms，没用到
	unsigned int I;
	for (I=0; I<800; I++); //3.5				650
}
void DelayB()
{//延时0.75ms
	unsigned int i;
	for (i=0; i<90; i++); // 0.75ms		93
}
void Delayms(unsigned int xms){
	unsigned int i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}