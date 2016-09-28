#include<regx52.h>
#include<stdlib.h>

//扫描键盘
#define ROW_0 P2_0
#define ROW_1 P2_1
#define ROW_2 P2_2
#define ROW_3 P2_3
#define COL_0 P2_4
#define COL_1 P2_5
#define COL_2 P2_6
#define COL_3 P2_7
//LCD1602各口
#define LCD_RS P3_0
#define LCD_RW P3_1
#define LCD_E P3_2
#define LCD_DATA P1
//用double存放键盘得到的运算符
#define PLUS -1
#define SUBSTRACT -2
#define MULTIPLY -3
#define DIVIDE -4
#define EQUAL -5
#define CLEAR -6

struct KeyVal{
	char Val;
	struct KeyVal *next;
};

struct KeyVal *head, *tail, *p;

//结果
double result = 0;
//存放输入的数据
char xdata keyData[30];
//存放解码keyData得到的数据
double xdata Val[20];
//判断符号是否存在
int isPlus = 0;
int isSubstract = 0;
int isMultiply = 0;
int isDivide = 0;

void main(){
	head = tail = NULL;
	
}



/*加法*/
double plus(double plusNum1, double plusNum2){
	return plusNum1 + plusNum2;
}

/*减法*/
double substract(double substractNum1, double substractNum2){
	return substractNum1 - substractNum2;
}

/*乘法*/
double multiply(double multiplyNum1, double multiplyNum2){
	return multiplyNum1 * multiplyNum2;
}

/*除法*/
double divide(double divideNum1, double divideNum2){
	return divideNum1 / divideNum2;
}

unsigned char getKey(){
	P2 = 0xFF;
	ROW_0 = 0;
	if(0==COL_0){
		return 7;
	}else if(0==COL_1){
		return 8;
	}else if(0==COL_2){
		return 9;
	}else if(0==COL_3){
		return '/';
	}
	P2 = 0xFF;
	ROW_1 = 0;
	if(0==COL_0){
		return 4;
	}else if(0==COL_1){
		return 5;
	}else if(0==COL_2){
		return 6;
	}else if(0==COL_3){
		return '*';
	}
	P2 = 0xFF;
	ROW_2 = 0;
	if(0==COL_0){
		return 1;
	}else if(0==COL_1){
		return 2;
	}else if(0==COL_2){
		return 3;
	}else if(0==COL_3){
		return '-';
	}
	P2 = 0xFF;
	ROW_3 = 0;
	if(0==COL_0){
		return 'c';
	}else if(0==COL_1){
		return 0;
	}else if(0==COL_2){
		return '+';
	}
	return 0xFF;
}

void getKeyVal(){
	char keyVal = getKey();
	if(keyVal != 0xFF){
		p = (struct KeyVal *)malloc(sizeof(struct KeyVal));
		switch(keyVal){
			case 0:
				p->Val = 0;
				break;
			case 1:
				p->Val = 1;
				break;
			case 2:
				p->Val = 2;
				break;
			case 3:
				p->Val = 3;
				break;
			case 4:
				p->Val = 4;
				break;
			case 5:
				p->Val = 5;
				break;
			case 6:
				p->Val = 6;
				break;
			case 7:
				p->Val = 7;
				break;
			case 8:
				p->Val = 8;
				break;
			case 9:
				p->Val = 9;
				break;
			case '+':
				p->Val = PLUS;
				break;
			case '-':
				p->Val = SUBSTRACT;
				break;
			case '*':
				p->Val = MULTIPLY;
				break;
			case '/':
				p->Val = DIVIDE;
				break;
			case 'c':
				p->Val = CLEAR;
				break;
			default:
				break;
		}
		p->next = NULL;
		if(head == NULL){
			head = tail = p;
		}else{
			tail->next = p;
			tail = p;
		}
	}
}

void comput(){
	
}