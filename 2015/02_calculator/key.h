#include<reg52.h>

void jianpan();
void delay_ms(uint i);
uchar key;

void jianpan(){
	uchar temp;
	P3 = 0xfe;
	temp = P3;
	if(temp != 0xf0){
		delayy_ms(10);
		temp = P3;
		temp = temp & 0xf0;
		if(temp != 0xfe){
			temp = P3;
			while(P3 != 0xfe);//*********************
			switch(temp){
			case 0xee:
				key = 1;
				break;
			case 0xde:
				key = 5;
				break;
			case 0xbe:
				key = 9;
				break;
			case 0x7e:
				key = 13;
				break;
			}
		}
	}
	P3 = 0xfd;
	temp = P3;
	temp = temp & 0xf0;
	if(temp != 0xf0){
		delay_ms(5);
		if(temp != 0xf0){
			temp = P3;
			while(P3 != 0xfd);
			switch(temp){
			case 0xed:
				key = 2;
				break;
			case 0xdd:
				key = 6;
				break;
			case 0xbd:
				key = 0;
				break;
			case 0x7d:
				key = 14;
				break;
			}
		}
	}
	P3 = 0xfb;
	temp = P3;
	temp = temp & 0xf0;
	if(temp != 0xf0){
		delay_ms(5);
		if(temp != 0xf0){
			temp = P3;
			while(P3 != 0xfb);
			switch(temp){
			case 0xeb:
				key = 3;
				break;
			case 0xdb:
				key = 7;
				break;
			case 0xbb:
				key = 11;
				break;
			case 0x7b:
				key = 15;
				break;
			}
		}
	}
	P3 = 0xf7;
	temp = P3;
	temp = temp & 0xf0;
	if(temp != 0xf0){
		delay_ms(5);
		if(temp != 0xf0){
			temp = P3;
			while(P3 != 0xf7);
			switch(temp){
			case 0xe7:
				key = 4;
				break;
			case 0xd7:
				key = 8;
				break;
			case 0xb7:
				key = 12;
				break;
			case 0x77:
				key = 16;
				break;
			}
		}
	}
}
