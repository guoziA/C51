#include "led.h";
#include<regx52.h>

#define LED P0

//#define LED_DIG1 P3_4
//#define LED_DIG2 P3_5
//#define LED_DIG3 P3_6
//#define LED_DIG4 P3_7


//char code number[] = {//0,1,2,3,4,5,6,7,8,9
//	0xfa, 0x22, 0xb9, 0xab, 0x63, 0xcb, 0xdb, 0xa2, 0xfb, 0xeb
//};

//char code letter[] = {//D,C,E,R,space,g,o,d
//	0xfa, 0xd8, 0xd9, 0xf3, 0x00, 0xeb, 0xe1, 0x3b
//};

//开发板测试********************************************
#define LED_DIG1 P1_3
#define LED_DIG2 P1_2
#define LED_DIG3 P1_1
#define LED_DIG4 P1_0


char code number[] = {//0,1,2,3,4,5,6,7,8,9
	0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11
};

char code letter[] = {//D,C,E,R,space
	0xfa, 0xd8, 0xd9, 0xf3, 0xff
};
//**************************************************************


char char_code[4];


void LED_stop(){
	LED = 0X00;
	LED_DIG1 = 1;
	LED_DIG2 = 1;
	LED_DIG3 = 1;
	LED_DIG4 = 1;
	
//	LED = 0X00;
//	LED_DIG1 = 0;
//	LED_DIG2 = 0;
//	LED_DIG3 = 0;
//	LED_DIG4 = 0;
}


void chanslate(char* str){//将字符串翻译成可以显示的code
	unsigned char _j;
	for(_j = 0; _j < 4; _j++){
		if(*str == '1'){
			char_code[_j] = number[1];
		}
		else if(*str == '2'){
			char_code[_j] = number[2];
		}
		else if(*str == '3'){
			char_code[_j] = number[3];
		}
		else if(*str == '4'){
			char_code[_j] = number[4];
		}
		else if(*str == '5'){
			char_code[_j] = number[5];
		}
		else if(*str == '6'){
			char_code[_j] = number[6];
		}
		else if(*str == '7'){
			char_code[_j] = number[7];
		}
		else if(*str == '8'){
			char_code[_j] = number[8];
		}
		else if(*str == '9'){
			char_code[_j] = number[9];
		}
		else if(*str == '0'){
			char_code[_j] = number[0]; // 如果0显示有问题，在这里**************
		}
		else if(*str == 'D'){
			char_code[_j] = letter[0];
		}
		else if(*str == 'C'){
			char_code[_j] = letter[1];
		}
		else if(*str == 'E'){
			char_code[_j] = letter[2];
		}
		else if(*str == 'R'){
			char_code[_j] = letter[3];
		}
		else if(*str == ' '){
			char_code[_j] = letter[4];
		}
		else if(*str == 'g'){
			char_code[_j] = letter[5];
		}
		else if(*str == 'o'){
			char_code[_j] = letter[6];
		}
		else{
			char_code[0] = letter[2];
			char_code[1] = letter[3];
			char_code[2] = letter[3];
			char_code[3] = letter[0];
			return;
		}
		str++;
	}
}

void display(char* dis_data){//显示

	unsigned int _i;

	chanslate(dis_data);
	LED_stop();
	
	LED = char_code[0];
	LED_DIG1 = 0;
	for(_i = 10; _i > 0; _i--);
	LED_DIG1 = 1;
	LED = 0X00;
	
	LED = char_code[1];
	LED_DIG2 = 0;
	for(_i = 10; _i > 0; _i--);
	LED_DIG2 = 1;
	LED = 0X00;
	
	LED = char_code[2];
	LED_DIG3 = 0;
	for(_i = 10; _i > 0; _i--);
	LED_DIG3 = 1;
	LED = 0X00;
	
	LED = char_code[3];
	LED_DIG4 = 0;
	for(_i = 10; _i > 0; _i--);
	LED_DIG4 = 1;
	LED = 0X00;
}

void LED_init(){//LED初始化
	int led_init_time = 500;
	P3 = (P3 && 0X0F);
	P0 = 0X00;
	while(led_init_time--);
}

void welcome(){
	unsigned int welcome_i = 0, welcome_j = 0;
	
	LED_stop();
	
	for(welcome_i = 0; welcome_i < 3; welcome_i++){
				
//		LED = letter[5];
//		LED_DIG1 = 0;
//		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
//		LED_DIG1 = 1;
//		LED = 0X00;
//		
//		LED = letter[6];
//		LED_DIG2 = 0;
//		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
//		LED_DIG2 = 1;
//		LED = 0X00;
//		
//		LED = letter[6];
//		LED_DIG3 = 0;
//		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
//		LED_DIG3 = 1;
//		LED = 0X00;
//		
//		LED = letter[7];
//		LED_DIG4 = 0;
//		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
//		LED_DIG4 = 1;
//		LED = 0X00;
//		for(welcome_j = 30000; welcome_j > 0; welcome_j--);
//		}

				
		LED = number[1];
		LED_DIG1 = 0;
		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
		LED_DIG1 = 1;
		LED = 0Xff;
		
		LED = number[2];
		LED_DIG2 = 0;
		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
		LED_DIG2 = 1;
		LED = 0Xff;
		
		LED = number[3];
		LED_DIG3 = 0;
		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
		LED_DIG3 = 1;
		LED = 0Xff;
		
		LED = number[4];
		LED_DIG4 = 0;
		for(welcome_j = 15000; welcome_j > 0; welcome_j--);
		LED_DIG4 = 1;
		LED = 0Xff;
		
		for(welcome_j = 30000; welcome_j > 0; welcome_j--);
	}
	
	
	
	display("    ");

}