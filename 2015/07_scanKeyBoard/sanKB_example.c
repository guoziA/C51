#include<regx52.h>

#define ROW_0 P3_4
#define ROW_1 P3_5
#define ROW_2 P3_6
#define ROW_3 P3_7
#define COLUMN_0 P3_0
#define COLUMN_1 P3_1
#define COLUMN_2 P3_2
#define COLUMN_3 P3_3

code unsigned char num[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80}; 
code unsigned char test[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char keyVal = 0xff;

void delay(unsigned int);
void showNum(unsigned int numVal);
void getKey();

void delay(unsigned int i){
	for( ; i>0; i--);
}

void showNum(unsigned int numVal){
	unsigned char temp;
	unsigned char i;
	for(i=0; i<8; i++){
		P2 = 0x00;
	if((0==numVal)&&(i!=0)){
			break;
		}
		temp = numVal%10;
		P1 = (0x01<<i);
		P2 = num[temp];
		numVal = numVal/10;
		delay(100);
	}
}

void getKey(){
	P3 = 0xFF;
	ROW_0 = 0;
	if(0==COLUMN_0){
		keyVal =  0;
	}else if(0==COLUMN_1){
		keyVal = 1;
	}else if(0==COLUMN_2){
		keyVal = 2;
	}else if(0==COLUMN_3){
		keyVal = 3;
	}
	P3 = 0xFF;
	ROW_1 = 0;
	if(0==COLUMN_0){
		keyVal = 4;
	}else if(0==COLUMN_1){
		keyVal = 5;
	}else if(0==COLUMN_2){
		keyVal = 6;
	}else if(0==COLUMN_3){
		keyVal = 7;
	}
	P3 = 0xFF;
	ROW_2 = 0;
	if(0==COLUMN_0){
		keyVal = 8;
	}else if(0==COLUMN_1){
		keyVal = 9;
	}else if(0==COLUMN_2){
		keyVal = 10;
	}else if(0==COLUMN_3){
		keyVal = 11;
	}
	P3 = 0xFF;
	ROW_3 = 0;
	if(0==COLUMN_0){
		keyVal = 12;
	}else if(0==COLUMN_1){
		keyVal = 13;
	}else if(0==COLUMN_2){
		keyVal = 14;
	}else if(0==COLUMN_3){
		keyVal = 15;
	}
//	return 0xFF;
}
void main(){

	while(1){
		getKey();
		if(0xFF == keyVal){
			P1 = ~0x00;
			P2 = num[0];
		}else{
			showNum(keyVal);
		}
	}
}
	