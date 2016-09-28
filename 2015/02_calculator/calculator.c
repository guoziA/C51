#include<regx52.h>
#include<math.h>

//扫描键盘，等号用外中断P33/INIT1
#define ROW_0 P2_7
#define ROW_1 P2_6
#define ROW_2 P2_5
#define ROW_3 P2_4
#define COL_0 P2_3
#define COL_1 P2_2
#define COL_2 P2_1
#define COL_3 P2_0

//LCD1602各口
#define LCD_RS P3_0
#define LCD_RW P3_1
#define LCD_E P3_2
#define LCD_Data P1
#define LCD_Busy P1_7

//用double存放键盘得到的运算符
#define PLUS 1.051
#define SUBSTRACT 1.052
#define MULTIPLY 1.053
#define DIVIDE 1.054
#define EMPTY_KEY_DATA '$'
#define EMPTY_VAL 1.055
#define AFTER_OPERATOR '~'

#define EVERY50MS 50000

//结果
double result = 0;
unsigned char strLength;
unsigned char keyData_i;
//存放输入的数据
char xdata keyData[30];
//存放解码keyData得到的数据
double xdata Val[30];
char resultToShow[] = "                ";
char inputToShow[] = "                ";
char hasEqual;
unsigned char timer0_count, show_input, show_x;

void equalInit();
void keyData_Init();
void fill_keyData();
char getKey();
void translate_keyData();
void comput();
void delay(unsigned char);
void delayms(unsigned int);
char *doubleToString(double);

void LCD_busy();//LCD忙检测
void LCD_WriteCode(unsigned char cmd);//向LCD写命令
void LCD_WriteData(unsigned char dat);//向LCD写数据
void LCD_GoToXY(unsigned char x,unsigned char y);//定位到点(x,y)
void LCD_ShowString(unsigned char x,unsigned char y, char *str);//从（x，y）写字符串str
void LCD_Init();//LCD初始化
void LCD_Clear();//清屏
void resultToShowInit();
void fill_inputToShow();
void system_reset();
void timer0Init();


void main(){
	system_reset();//系统复位
	while(1){
		fill_keyData();
//		comput();
//		doubleToString(result);
	}
}

//void fill_inputToShow(){
//	unsigned char keyData_i, show_i = 0;
//	char temp = EMPTY_KEY_DATA;
//	for(keyData_i = 0; keyData_i < 30; keyData_i++){
//		if(keyData[keyData_i] == EMPTY_KEY_DATA) break;
//		if(show_i < 15){
//			inputToShow[show_i] = keyData[keyData_i];
//			show_i++;
//			if(inputToShow[show_i] == AFTER_OPERATOR)
//				show_i--;
//		}else{
//			if(keyData[keyData_i] != AFTER_OPERATOR){
//				temp = keyData[keyData_i];//第16个
//			}
//			if(temp != EMPTY_KEY_DATA){
//				for(show_i = 0; show_i < 14; show_i++){
//					inputToShow[show_i] = inputToShow[show_i + 1];
//				}
//				inputToShow[15] = temp;
//			}
//			show_i = 15;
//		}
//	}
//	show_x = show_i;
//}

void timer0() interrupt 1{
	TH0 = (65536 -EVERY50MS) / 256;
	TL0 = (65536 - EVERY50MS) % 256;
	timer0_count++;
//	fill_inputToShow();
	LCD_ShowString(0,0,keyData);
	if(timer0_count == 15){
		P0_7 = !P0_7;
		timer0_count = 0;
		show_input = !show_input;
		if(!show_input){
			LCD_ShowString(show_input,0,"_");
		}else{
			LCD_ShowString(show_input,0," ");
		}
	}
}

void system_reset(){
	//初始化keyData为$
	keyData_Init();
	//初始化等键，外中断1
	equalInit();
	//初始化定时器0，用于显示
	timer0Init();
	//初始化屏幕
	LCD_Init();
	//初始化显示结果的数组为空格
	resultToShowInit();
	//判断=是否已经按下
	hasEqual = 0;
	//用于记录按键的变量
	keyData_i = 0;
	timer0_count = 0;
	show_input = 0;
	show_x = 0;
}

void timer0Init(){
	TMOD = 0X01;//定时器1工作方式1
	TH0 = (65536 - EVERY50MS) / 256;
	TL0 = (65536 - EVERY50MS) % 256;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

//初始化显示数组
void resultToShowInit(){
	unsigned char rts_i;
	for(rts_i = 0; rts_i < 16; rts_i++){
		resultToShow[rts_i] = ' ';
	}
}
//把double型的结果转变为可以显示的字符串
char *doubleToString (double myDouble){
	unsigned char i_s = 0;
	//得到整数部分
	int myInt = myDouble;
	//得到小数部分
	double myDecimals = myDouble - myInt;
	//取小数部分的4位
	int my4Decimals = myDecimals * 10000;
	int my3Decimals;
	int a = 0;//进位
	if(my4Decimals % 10 > 4) a = 1;
	my3Decimals = my4Decimals / 10 + a;
	//进到个位
	if(my3Decimals % 1000 == 0) {
		myInt = myInt + 1;
		my3Decimals = 0;
	}
	//放入resultToShow
	if(my3Decimals != 0){
		for(;i_s < 3; i_s++){
			resultToShow[15-i_s] = (char)(my3Decimals % 10) + '0';
			my3Decimals = my3Decimals / 10;
		}
		//小数点
	resultToShow[12] = '.';
	i_s = 4;
	}
	
	//整数部分
	if(myInt != 0){
		while(myInt){
			resultToShow[15-i_s] = (char)(myInt % 10) + '0';
			myInt = myInt / 10;
			i_s++;
		}
	}else{
		if(myDecimals == 0){
			resultToShow[15] = '0';
		}else{
			resultToShow[10] = '0';
		}
		i_s++;
	}
	return resultToShow;
}


/*外中断0，等号*/
void equal() interrupt 2{
	if(!hasEqual){
		comput();
		doubleToString(result);
		LCD_ShowString(0,1,resultToShow);
		hasEqual = 1;
	}
}

/*解读从键盘得到是数据，装入到Val[]中，得到val的格式：num operator num operator num...全部以double表示*/
void translate_keyData(){
	int i, j, k;
//	keyData[0] = 1;
//	keyData[1] = 1;
//	keyData[2] = '+';
//	keyData[3] = AFTER_OPERATOR;
//	keyData[4] = 1;
//	keyData[5] = 1;
//	keyData[6] = 1;
	for(i = 0; i < 50; i++){
		//如果keyData是EMPTU_KEY_DATA说明前一个就是最后一个
		if(keyData[i] == EMPTY_KEY_DATA){
			for(j = i-1; keyData[j] != AFTER_OPERATOR && j > 0; j--){
				if(Val[k] == EMPTY_VAL) Val[k] = 0;
				Val[k] = Val[k] + keyData[j] *pow(10,i-j-1);
			}
			break;
		}
		//遇到符号的处理keyData的格式为：num(1 2 3...) operator AFTER_OPERATOR num(1 2 3 ...) operator AFTER_OPERATOR num(1 2 3) ...
		if((keyData[i] == '+') || (keyData[i] == '-') || (keyData[i] == '*') || (keyData[i] == '/')){
			//把数字的个位、十位、百位。。。转变为一个数，并放入Val中
			for(j = i; keyData[j] != AFTER_OPERATOR && j > 0; j--){
				if(Val[k] == EMPTY_VAL) Val[k] = 0;
				Val[k] = Val[k] + keyData[j-1] *pow(10,i-j);
			}
			k++;
			//把相应的符号也通过编辑好的常数放入val中
			switch(keyData[i]){
				case '+' :
					Val[k] = PLUS;
					k++;
					break;
				case '-' :
					Val[k] = SUBSTRACT;
					k++;
					break;
				case '*' :
					Val[k] = MULTIPLY;
					k++;
					break;
				case '/' :
					Val[k] = DIVIDE;
					k++;
					break;
				default:
					break;
			}
		}
	}
}

/*计算*/
void comput(){
	char i_c;
	//翻译keyData得到Val
	translate_keyData();
	//首先计算乘法和除法
	
//	Val[0] = 1;
//	Val[1] = PLUS;
//	Val[2] = 3;
//	Val[3] = MULTIPLY;
//	Val[4] = 2;
//	Val[5] = SUBSTRACT;
//	Val[6] = 1;
//	Val[7] = DIVIDE;
//	Val[8] = 2;
//	Val[9] = MULTIPLY;
//	Val[10] = 4;
	for(i_c = 0; i_c < 30; i_c++){
		if(Val[i_c] == EMPTY_VAL) break;
		//乘号，将乘号前后的数相乘，返回给这两个数的后一个，前一个赋0
		//比如Val[0] + Val[2] - Val[4] * Val[6] + Val[8] / Val[10] * Val[12]
		if(Val[i_c] == MULTIPLY){
			Val[i_c+1] = Val[i_c-1] * Val[i_c+1];
			Val[i_c-1] = 0;
			//第一次进入Val[0] + Val[2] - 0 + Val[6] * Val[8] + ...
			if(i_c - 2 > 0){
				if(Val[i_c-2] == SUBSTRACT){
					Val[i_c+1] = -Val[i_c+1];
					//Val[0] + Val[2] - 0 - Val[6] * Val[8] ...
				}
			}
			//除号
		}else if(Val[i_c] == DIVIDE){
			Val[i_c+1] = Val[i_c-1]/Val[i_c+1];//divide(Val[i-1], Val[i+1]);
			Val[i_c-1] = 0;
			if(i_c - 2 > 0){
				if(Val[i_c-2] == SUBSTRACT){
					Val[i_c+1] = -Val[i_c+1];
				}
			}
		}
	}
	i_c = 0;//i返回0
	//计算减法,减号后面的数变为负数就行
	while(Val[i_c] != EMPTY_VAL){
		if(Val[i_c] == SUBSTRACT){
			Val[i_c+1] = -Val[i_c +1];
		}
		i_c++;
	}
	i_c = 0;
	//把不是符号的Val全相加
	while(Val[i_c] != EMPTY_VAL){
		if((Val[i_c] != PLUS) && (Val[i_c] != SUBSTRACT) && (Val[i_c] != MULTIPLY) && (Val[i_c] != DIVIDE)){
			result = result + Val[i_c];
		}
		i_c++;
	}
}
/*初始化外中断0*/
void equalInit(){
	IT1 = 1;//外中断1下降沿触发
	EX1 = 1;//使能外中断1
	EA = 1;//总能中断
}

/*初始化存放输入数据的char数组，用于判断总输入长度*/
void keyData_Init(){
	unsigned char i;
	for(i = 0; i < 30; i++){
		keyData[i] = EMPTY_KEY_DATA;
		Val[i] = EMPTY_VAL;
	}
}



/*键盘扫描*/
char getKey(){
	P2 = 0xFF;
	ROW_0 = 0;
	if(0==COL_0){
		while(!COL_0);
		return 7;
	}else if(0==COL_1){
		while(!COL_1);
		return 8;
	}else if(0==COL_2){
		while(!COL_2);
		return 9;
	}else if(0==COL_3){
		while(!COL_3);
		return '/';
	}
	P2 = 0xFF;
	ROW_1 = 0;
	if(0==COL_0){
		while(!COL_0);
		return 4;
	}else if(0==COL_1){
		while(!COL_1);
		return 5;
	}else if(0==COL_2){
		while(!COL_2);
		return 6;
	}else if(0==COL_3){
		while(!COL_3);
		return '*';
	}
	P2 = 0xFF;
	ROW_2 = 0;
	if(0==COL_0){
		while(!COL_0);
		return 1;
	}else if(0==COL_1){
		while(!COL_1);
		return 2;
	}else if(0==COL_2){
		while(!COL_2);
		return 3;
	}else if(0==COL_3){
		while(!COL_3);
		return '-';
	}
	P2 = 0xFF;
	ROW_3 = 0;
	if(0==COL_0){
		while(!COL_0);
		return 'c';//清屏复位
	}else if(0==COL_1){
		while(!COL_1);
		return 0;
	}else if(0==COL_2){
		while(!COL_2);
		return '+';
	}else if(0==COL_3){
		while(!COL_3);
		return 's';//副功能
	}
	return 10;//没有按键
}

//把通过键盘得到的数据填入到keyData 
void fill_keyData(){
	char KeyVal = 10;
//	KeyVal = '+';
	KeyVal = getKey();
	//有键被按下,给keyData赋值
	if(KeyVal != 10){
//		inputToShow[show_input] = KeyVal + '0';
//		LCD_WriteData(KeyVal = '0');
//		show_input++;
		keyData[keyData_i] = KeyVal;
		keyData_i++;
		//按下的键是符号键，在后面加上符号键标志AFTER_OPERATOR
		if((KeyVal == '+') || (KeyVal == '-') || (KeyVal == '*') || (KeyVal == '/')){
			keyData[keyData_i] = AFTER_OPERATOR;
			keyData_i++;
		}
	}
}


void delay(unsigned char x){//约60us
	unsigned char i;
	for(i = x*5; i > 0; i--);
}

void delayms(unsigned int xms){
	unsigned int i_d, j_d;
	for(i_d = xms; i_d > 0; i_d--)
		for(j_d = 110; j_d > 0; j_d--);
}

/*LCD忙检测*/
void LCD_busy(){
	bit busy;
	LCD_E = 0;
    LCD_RS = 0;//选择指令寄存器
    LCD_RW = 1;  //读信号线
    do{
		//先将LCD_Busy置高
	   LCD_Busy = 1;
		//使能
	   LCD_E = 1;
	   delay(1);
		//检测忙信号
	   busy = LCD_Busy;
	   LCD_E = 0;
	   }while(busy);//不忙才退出
}

//写命令
void LCD_WriteCode(unsigned char cmd){
	//判忙
   LCD_busy();
	//写操作，下降沿使能
   LCD_E = 0;
	//选择命令寄存器
   LCD_RS = 0;
	//写入操作
   LCD_RW = 0;
	//cmd写入LCD_Data
   LCD_Data = cmd; 
   LCD_E = 1;
	 delay(1);
   LCD_E = 0;	
	//从1到0产生一个下降沿使命令写入
}

//光标移动到xy位置
void LCD_GoToXY(unsigned char x,unsigned char y)
{
   if(y == 0){
	  LCD_WriteCode(0x80 | x);
   }else{
      LCD_WriteCode(0xC0 | x);
   }
}

//写数据
void LCD_WriteData(unsigned char dat){
	//判忙
   LCD_busy();
	//寄存器选择控制，写入数据寄存器
   LCD_RS = 1;	  
   LCD_RW = 0;
	//写入数据
   LCD_Data = dat;
	//下降沿使能
   LCD_E = 1;
	delay(1);
   LCD_E = 0;
}

//初始化屏幕
void LCD_Init(){
   LCD_E = 0;
   LCD_WriteCode(0x08);
   LCD_Clear(); 
   LCD_WriteCode(0x38);
   LCD_WriteCode(0x06);
   LCD_WriteCode(0x0c);

}

//清屏
void LCD_Clear(){
   LCD_WriteCode(0x01);
}

//显示字符str
void LCD_ShowString(unsigned char x,unsigned char y,char *str){
	LCD_GoToXY(x,y);
	while(*str){
		LCD_WriteData(*str);
		str++;
	}
}