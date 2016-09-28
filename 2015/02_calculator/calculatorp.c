#include<regx52.h>
#include<math.h>
#include<stdlib.h>

//ɨ����̣��Ⱥ������ж�P33/INIT1
#define ROW_0 P2_7
#define ROW_1 P2_6
#define ROW_2 P2_5
#define ROW_3 P2_4
#define COL_0 P2_3
#define COL_1 P2_2
#define COL_2 P2_1
#define COL_3 P2_0

//LCD1602����
#define LCD_RS P3_0
#define LCD_RW P3_1
#define LCD_E P3_2
#define LCD_Data P1
#define LCD_Busy P1_7

//��double��ż��̵õ��������
#define PLUS 1.051
#define SUBSTRACT 1.052
#define MULTIPLY 1.053
#define DIVIDE 1.054
#define EMPTY_KEY_DATA '$'
#define EMPTY_VAL 1.055
#define AFTER_OPERATOR '~'
#define EMPTY_INPUT '&'

#define EVERY50MS 50000

//���
double result = 0;
unsigned char strLength;
char keyData_i;
//������������
char xdata keyData[30];
//��Ž���keyData�õ�������
double xdata Val[30];
char resultToShow[] = "                ";
char inputToShow[30];
char hasEqual;
unsigned char timer0_count, show_length, show_x, show_input, show_input_i, ERROR;

void equalInit();
void keyData_Init();
void fill_keyData();
char getKey();
void translate_keyData();
void comput();
void delay(unsigned char);
void delayms(unsigned int);
char *doubleToString(double);

void LCD_busy();//LCDæ���
void LCD_WriteCode(unsigned char cmd);//��LCDд����
void LCD_WriteData(unsigned char dat);//��LCDд����
void LCD_GoToXY(unsigned char x,unsigned char y);//��λ����(x,y)
void LCD_ShowString(unsigned char x,unsigned char y, char *str);//�ӣ�x��y��д�ַ���str
void LCD_ShowInf();
void LCD_Init();//LCD��ʼ��
void LCD_Clear();//����
void resultToShowInit();
void fill_inputToShow();
void system_reset();
void timer0Init();
void createInput_show();

//struct input_show{
//	char showchar;
//	struct input_show *next;
//};

//struct input_show *head, *tail;

void main(){
	system_reset();//ϵͳ��λ
	while(1){
		fill_keyData();

//		comput();
//		doubleToString(result);

	}
}


//void createInput_show(){
//	unsigned char cs_i = show_x;
//	for(; cs_i < 30; cs_i++){
//		if(keyData[cs_i] == EMPTY_KEY_DATA) break;
//		if(keyData[cs_i] != AFTER_OPERATOR){
//			struct input_show *p;
//			p = (struct input_show *)malloc(sizeof(struct input_show));
//			switch(keyData[cs_i]){
//				case 0:
//					p->showchar = 0 + '0';
//					break;
//				case 1:
//					p->showchar = 1 + '0';
//					break;
//				case 2:
//					p->showchar = 2 + '0';
//					break;
//				case 3:
//					p->showchar = 3 +'0';
//					break;
//				case 4:
//					p->showchar = 4 + '0';
//					break;
//				case 5:
//					p->showchar = 5 + '0';
//					break;
//				case 6:
//					p->showchar = 6 +'0';
//					break;
//				case 7:
//					p->showchar = 7 + '0';
//					break;
//				case 8:
//					p->showchar = 8 +'0';
//					break;
//				case 9:
//					p->showchar = 9 + '0';
//					break;
//				case '+':
//					p->showchar = '+';
//					break;
//				case '-':
//					p->showchar = '-';
//					break;
//				case '*':
//					p->showchar = '*';
//					break;
//				case '/':
//					p->showchar = '/';
//					break;
//				case 'c':
//					p->showchar = 'c';
//					break;
//				default:
//					break;
//			}
//			p->next = NULL;
//			if(head == NULL){
//				head = tail = p;
//			}else{
//				tail->next = p;
//				tail = p;
//			}
//		}
//	}
//}

//��ʾ������ʽ
void LCD_ShowInf(){
	unsigned char length = 0;
	char showchar = 0, is_i = 0;
	//��ʾ��1��1��
	LCD_GoToXY(0,0);
	//����ʾ��ֵ
	while(inputToShow[is_i] != EMPTY_INPUT){
		//�������ʽλ������16��ֻ��ʾ�����16λ
		if(show_input_i < 16){
		LCD_WriteData(inputToShow[is_i]);
		}else{
			if(inputToShow[show_input_i - 15 +is_i] != EMPTY_INPUT){
			LCD_WriteData(inputToShow[show_input_i - 15 + is_i]);
			}
		}
		is_i++;
		length++;
	}
	//����ʾ�ĸ�������show_x���ڹ����ʾ
	show_x = length;
}

void timer0() interrupt 1{
	TH0 = (65536 -EVERY50MS) / 256;
	TL0 = (65536 - EVERY50MS) % 256;
	timer0_count++;
	//ÿ50msˢ��һ����ʾ
	LCD_ShowInf();
	//ÿ15 x 50ms�����˸һ��
	if(timer0_count == 15){
		P0_7 = !P0_7;
		timer0_count = 0;
		//�����жϴ�ʱ��û�й��
		show_input = !show_input;
		if(show_x > 15) //���Ҫ��ʾ�����һλ
			show_x = 15;
		if(!show_input && !hasEqual){
			LCD_ShowString(show_x,0,"_");
		}else{
			LCD_ShowString(show_x,0," ");
		}
	}
}

void system_reset(){
	//��ʼ��keyDataΪ$
	keyData_Init();
	//��ʼ���ȼ������ж�1
	equalInit();
	//��ʼ����ʱ��0��������ʾ
	timer0Init();
	//��ʼ����Ļ
	LCD_Init();
	//��ʼ����ʾ���������Ϊ�ո�
	resultToShowInit();
	//�ж�=�Ƿ��Ѿ�����
	hasEqual = 0;
	//���ڼ�¼�����ı���
	keyData_i = 0;
	timer0_count = 0;
	show_length = 0;
	show_input = 0;
	show_input_i = 0;
	ERROR = 0;
}

void timer0Init(){
	TMOD = 0X01;//��ʱ��1������ʽ1
	TH0 = (65536 - EVERY50MS) / 256;
	TL0 = (65536 - EVERY50MS) % 256;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

//��ʼ����ʾ����
void resultToShowInit(){
	unsigned char rts_i;
	for(rts_i = 0; rts_i < 16; rts_i++){
		resultToShow[rts_i] = ' ';
	}
}
//��double�͵Ľ��ת��Ϊ������ʾ���ַ���
char *doubleToString (double myDouble){
	unsigned char i_s = 0;
	//�õ���������
	int myInt = myDouble;
	//�õ�С������
	double myDecimals = myDouble - myInt;
	//ȡС�����ֵ�4λ
	int my4Decimals = myDecimals * 10000;
	int my3Decimals;
	int a = 0;//��λ
	if(my4Decimals % 10 > 4) a = 1;
	my3Decimals = my4Decimals / 10 + a;
	//������λ
	if(my3Decimals % 1000 == 0 && my3Decimals != 0) {
		myInt = myInt + 1;
		my3Decimals = 0;
	}
	//����resultToShow
	if(my3Decimals != 0){
		for(;i_s < 3; i_s++){
			resultToShow[15-i_s] = (char)(my3Decimals % 10) + '0';
			my3Decimals = my3Decimals / 10;
		}
		//С����
	resultToShow[12] = '.';
	i_s = 4;
	}
	
	//��������
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


/*���ж�0���Ⱥ�*/
void equal() interrupt 2{
	//���ж�=���Ƿ��Ѿ�����
	if(!hasEqual){
		//����
		comput();
		//�������ַ���
		if(result < 0){
			ERROR = 1;
		}
		doubleToString(result);
		//��ʾ���
		if(!ERROR){
		LCD_ShowString(0,1,resultToShow);
		}else{
			LCD_ShowString(5,1,"ERROR");
		}
		hasEqual = 1;
	}
}

/*����Ӽ��̵õ������ݣ�װ�뵽Val[]�У��õ�val�ĸ�ʽ��num operator num operator num...ȫ����double��ʾ*/
void translate_keyData(){
	int i, j, k;
//	keyData[0] = 1;
//	keyData[1] = '+';
//	keyData[2] = AFTER_OPERATOR;
//	keyData[3] = 3;
//	keyData[4] = '*';
//	keyData[5] = AFTER_OPERATOR;
//	keyData[6] = 2;
//	keyData[7] = '-';
//	keyData[8] = AFTER_OPERATOR;
//	keyData[9] = 1;
//	keyData[10] = '/';
//	keyData[11] = AFTER_OPERATOR;
//	keyData[12] = 2;
//	keyData[13] = '*';
//	keyData[14] =AFTER_OPERATOR;
//	keyData[15] = 4;
	for(i = 0; i < 30; i++){
		//���keyData��EMPTU_KEY_DATA˵��ǰһ���������һ��
		if(keyData[i] == EMPTY_KEY_DATA){
			for(j = i-1; keyData[j] != AFTER_OPERATOR && j > 0; j--){
				if(Val[k] == EMPTY_VAL) Val[k] = 0;
				Val[k] = Val[k] + keyData[j] *pow(10,i-j-1);
			}
			break;
		}
		//�������ŵĴ���keyData�ĸ�ʽΪ��num(1 2 3...) operator AFTER_OPERATOR num(1 2 3 ...) operator AFTER_OPERATOR num(1 2 3) ...
		if((keyData[i] == '+') || (keyData[i] == '-') || (keyData[i] == '*') || (keyData[i] == '/')){
			//�����ֵĸ�λ��ʮλ����λ������ת��Ϊһ������������Val��
			for(j = i-1; (keyData[j] != AFTER_OPERATOR) && (j >= 0); j--){
				if(Val[k] == EMPTY_VAL){
					Val[k] = 0;
				}
				Val[k] = Val[k] + keyData[j] *pow(10,i-j -1);
			}
			k++;
			//����Ӧ�ķ���Ҳͨ���༭�õĳ�������val��
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

/*����*/
void comput(){
	char i_c;
	//����keyData�õ�Val
	translate_keyData();
	//���ȼ���˷��ͳ���
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
		//�˺ţ����˺�ǰ�������ˣ����ظ����������ĺ�һ����ǰһ����0
		//����Val[0] + Val[2] - Val[4] * Val[6] + Val[8] / Val[10] * Val[12]
		if(Val[i_c] == MULTIPLY){
			Val[i_c+1] = Val[i_c-1] * Val[i_c+1];
			Val[i_c-1] = 0;
			//��һ�ν���Val[0] + Val[2] - 0 + Val[6] * Val[8] + ...
			if(i_c - 2 > 0){
				if(Val[i_c-2] == SUBSTRACT){
					Val[i_c+1] = -Val[i_c+1];
					//Val[0] + Val[2] - 0 - Val[6] * Val[8] ...
				}
			}
			//����
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
	i_c = 0;//i����0
	//�������,���ź��������Ϊ��������
	while(Val[i_c] != EMPTY_VAL){
		if(Val[i_c] == SUBSTRACT){
			Val[i_c+1] = -Val[i_c +1];
		}
		i_c++;
	}
	i_c = 0;
	//�Ѳ��Ƿ��ŵ�Valȫ���
	while(Val[i_c] != EMPTY_VAL){
		if((Val[i_c] != PLUS) && (Val[i_c] != SUBSTRACT) && (Val[i_c] != MULTIPLY) && (Val[i_c] != DIVIDE)){
			result = result + Val[i_c];
		}
		i_c++;
	}
}
/*��ʼ�����ж�0*/
void equalInit(){
	IT1 = 1;//���ж�1�½��ش���
	EX1 = 1;//ʹ�����ж�1
	EA = 1;//�����ж�
}

/*��ʼ������������ݵ�char���飬�����ж������볤��*/
void keyData_Init(){
	unsigned char i;
	for(i = 0; i < 30; i++){
		keyData[i] = EMPTY_KEY_DATA;
		Val[i] = EMPTY_VAL;
		inputToShow[i] = EMPTY_INPUT;
	}
}



/*����ɨ��*/
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
		return 'c';//������λ
	}else if(0==COL_1){
		while(!COL_1);
		return 0;
	}else if(0==COL_2){
		while(!COL_2);
		return '+';
	}else if(0==COL_3){
		while(!COL_3);
		return 's';//������
	}
	return 10;//û�а���
}

//��ͨ�����̵õ����������뵽keyData 
void fill_keyData(){
	char KeyVal = 10;
	char show_data;//��ʾ��
//	KeyVal = '+';
	if(!hasEqual){
		if(keyData_i > 29){
			LCD_ShowString(0,1,"limited,plis '='");
			return;
		}
		KeyVal = getKey();
		//�м�������,��keyData��ֵ
		if(KeyVal != 10){
			keyData[keyData_i] = KeyVal;
			keyData_i++;
//*************************************************************************8
			//���������������ʾ������
			show_data = KeyVal;
			if(show_data < 10){
				show_data = show_data + '0';
			}
			inputToShow[show_input_i] = show_data;
			show_input_i++;			
			
			//���µļ��Ƿ��ż����ں�����Ϸ��ż���־AFTER_OPERATOR
			if((KeyVal == '+') || (KeyVal == '-') || (KeyVal == '*') || (KeyVal == '/')){
				
				if(keyData[keyData_i - 2] < 0 || keyData[keyData_i - 2] >9){//��һ���Ƿ��Ż�������������һ�𣬴�����ʽ
					ERROR = 1;
				}
				keyData[keyData_i] = AFTER_OPERATOR;
				keyData_i++;
			}
		}
	}
}


void delay(unsigned char x){//Լ60us
	unsigned char i;
	for(i = x*5; i > 0; i--);
}

void delayms(unsigned int xms){
	unsigned int i_d, j_d;
	for(i_d = xms; i_d > 0; i_d--)
		for(j_d = 110; j_d > 0; j_d--);
}

/*LCDæ���*/
void LCD_busy(){
	bit busy;
	LCD_E = 0;
    LCD_RS = 0;//ѡ��ָ��Ĵ���
    LCD_RW = 1;  //���ź���
    do{
		//�Ƚ�LCD_Busy�ø�
	   LCD_Busy = 1;
		//ʹ��
	   LCD_E = 1;
	   delay(1);
		//���æ�ź�
	   busy = LCD_Busy;
	   LCD_E = 0;
	   }while(busy);//��æ���˳�
}

//д����
void LCD_WriteCode(unsigned char cmd){
	//��æ
   LCD_busy();
	//д�������½���ʹ��
   LCD_E = 0;
	//ѡ������Ĵ���
   LCD_RS = 0;
	//д�����
   LCD_RW = 0;
	//cmdд��LCD_Data
   LCD_Data = cmd; 
   LCD_E = 1;
	 delay(1);
   LCD_E = 0;	
	//��1��0����һ���½���ʹ����д��
}

//����ƶ���xyλ��
void LCD_GoToXY(unsigned char x,unsigned char y)
{
   if(y == 0){
	  LCD_WriteCode(0x80 | x);
   }else{
      LCD_WriteCode(0xC0 | x);
   }
}

//д����
void LCD_WriteData(unsigned char dat){
	//��æ
   LCD_busy();
	//�Ĵ���ѡ����ƣ�д�����ݼĴ���
   LCD_RS = 1;	  
   LCD_RW = 0;
	//д������
   LCD_Data = dat;
	//�½���ʹ��
   LCD_E = 1;
	delay(1);
   LCD_E = 0;
}

//��ʼ����Ļ
void LCD_Init(){
   LCD_E = 0;
   LCD_WriteCode(0x08);
   LCD_Clear(); 
   LCD_WriteCode(0x38);
   LCD_WriteCode(0x06);
   LCD_WriteCode(0x0c);

}

//����
void LCD_Clear(){
   LCD_WriteCode(0x01);
}

//��ʾ�ַ�str
void LCD_ShowString(unsigned char x,unsigned char y,char *str){
	LCD_GoToXY(x,y);
	while(*str){
		LCD_WriteData(*str);
		str++;
	}
}