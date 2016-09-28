#include <reg52.h>
#include <key.h>
#include <1602.h>

#define uchar unsigened char
#define uint unsigned int

void delay_ms(uint i);

uchar code a[] = "51 microcontrollers";
uchar code b[] = "calculator";

sbit wei = P2^7;
sbit du = P2^6;

uchar buf1[6] = {' ',' ',' ',' ',' ',' '};
uchar buf2[6] = {' ',' ',' ',' ',' ',' '};
uchar buf3[6];

uchar x = 0x30;
uchar flag1;
bit flag_x;
bit flag_y;
bit flag2;
uchar x1 = 0x30;

unsigned long int count1;
unsigned long int count2;
unsigned long int count3;

//手动分格模块
void delay_ms(uint i){
	unsigned char j;
	while(i--)
		for(j = 0; j < 110; j++);
}

void write_com(uint com){
	rs = 0;
	P0 = com;
	delay_ms(5);
	e = 1;
	delay_ms(5);
	e = 0;
}

void write_d(uchar d){
	rs = 1;
	P0 = d;
	delay_ms(5);
	e = 1;
	delay_ms(5);
	e = 0;
}

void init_1602(){
	rw = 0;
	write_com(0x38);
	delay_ms(5);
	write_com(0x0c);
	delay_ms(5);
	write_com(0x06);
	delay_ms(5);
	write_com(0x01);
	delay_ms(5);
	write_com(0x80);
	delay_ms(5);
}

//十进制转换
void conve(){
	if(key = 12){ //“=”***********key在哪里定义？
		if(buf1[5] != ' '){
			count1 = buf1[0] * 100000 + buf1[1] * 10000 + buf1[2] * 1000 + buf1[3] * 100 + buf1[4] * 10 + buf1[5];
		}else if(buf1[4] != ' '){
			count1 = buf1[0] * 10000 + buf1[1] * 1000 + buf1[2] * 100 + buf1[3] * 10 + buf1[4];
		}else if(buf1[3] != ' '){
			count1 = buf1[0] * 1000 + buf1[1] * 100 + buf1[2] * 10 + buf1[3];
		}else if(buf1[2] != ' '){
			count1 = buf1[0] * 100 + buf1[1] *10 + buf1[2];
		}else if(buf1[1] != ' '){
			count1 = buf1[0] * 10 + buf1[1];
		}else if(buf1[0] != ' '){
			count1 = buf1[0];
		}
	}
}

//计算
void calculate(){
	if(key == 12){//“=”
		switch(flag1){
		case 1:
			count3 = count1 + count2;
			flag_y = 1;
			break;
		case 2:
			count3 = count1 - count2;
			flag_y = 1;
			break;
		case 3:
			count3 = count1 * count2;
			flag_y = 1;
			break;
		case 4:
			if(count1 % count2 == 0){//整除
				count3 = count1 / count2;
				flag_y = 1;
			}else{//保留一位小数
				count3 = count1 * 10 / count2;
				flag_y = 1;
			}
			break;
		default:
			break;
				
		}
		//显示位
		buf3[0] = count3 / 100000;
		buf3[1] = count3 % 100000 / 10000;
		buf3[2] = count3 % 10000 / 1000;
		buf3[3] = count3 % 1000 / 100;
		buf3[4] = count3 % 100 / 10;
		buf3[5] = count3 % 10;
		if(flag_y == 1){
			if(buf3[0] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d(buf3[4] + x1);
				write_com(0xcd);	write_d(buf3[3] + x1);
				write_com(0xcc);	write_d(buf3[2] + x1);
				write_com(0xcb);	write_d(buf3[1] + x1);
				write_com(0xca);	write_d(buf3[0] + x1);
			}else if(buf3[1] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d(buf3[4] + x1);
				write_com(0xcd);	write_d(buf3[3] + x1);
				write_com(0xcc);	write_d(buf3[2] + x1);
				write_com(0xcb);	write_d(buf3[1] + x1);
			}else if(buf3[2] != 0){;
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d(buf3[4] + x1);
				write_com(0xcd);	write_d(buf3[3] + x1);
				write_com(0xcc);	write_d(buf3[2] + x1);
			}else if(buf3[3] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d(buf3[4] + x1);
				write_com(0xcd);	write_d(buf3[3] + x1);			
			}else if(buf3[4] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d(buf3[4] + x1);			
			}else if(buf3[5] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
			}
		}
		if(flag_y == 0){
			if(buf3[0] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d('.');
				write_com(0xcd);	write_d(buf3[4] + x1);
				write_com(0xcc);	write_d(buf3[3] + x1);
				write_com(0xcb);	write_d(buf3[2] + x1);
				write_com(0xca);	write_d(buf3[1] + x1);
				write_com(0x09);	write_d(buf3[0] + x1);
			}else if(buf3[1] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d('.');
				write_com(0xcd);	write_d(buf3[4] + x1);
				write_com(0xcc);	write_d(buf3[3] + x1);
				write_com(0xcb);	write_d(buf3[2] + x1);
				write_com(0xca);	write_d(buf3[1] + x1);			
			}else if(buf3[2] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d('.');
				write_com(0xcd);	write_d(buf3[4] + x1);
				write_com(0xcc);	write_d(buf3[3] + x1);
				write_com(0xcb);	write_d(buf3[2] + x1);			
			}else if(buf3[3] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d('.');
				write_com(0xcd);	write_d(buf3[4] + x1);
				write_com(0xcc);	write_d(buf3[3] + x1);			
			}else if(buf3[4] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);
				write_com(0xce);	write_d('.');
				write_com(0xcd);	write_d(buf3[4] + x1);			
			}else if(buf3[5] != 0){
				write_com(0xcf);	write_d(buf3[5] + x1);			
			}
			
		}
	}
}

void main(){
	uchar num;
	uchar i = 0, j = 0, m = 0;
	wei = 1;
	du = 1;
	P0 = 0x00;
	init_1602();
	write_com(0x80);
	for(num = 0; num < 16; num++){
		write_d(a[num]);
		delay_ms(5);
	}
	write_com(0xc0);
	for(num = 0; num < 16; num++){
		write_d(b[num]);
		delay_ms(5);
	}
	delay_ms(1000);
	write_com(0x01);
	delay_ms(5);
	while(1){
		key = ' ';
		jianpan();
		if(key >= 0){
			if(key <= 9){
				if(flag1 = 0){
					buf1[i] = key;
					i++;
					if(i == 7) i = 0;
				}
				if(flag1 != 0){
					buf2[j] = key;
					j++;
					if(j == 7) j = 0;
				}
			}
		}
		if(flag1 == 0){
			write_com(0x8f - i + 1);	write_d(buf1[0] + x1);
			write_com(0x8f - i + 2);	write_d(buf1[1] + x1);
			write_com(0x8f - i + 3);	write_d(buf1[2] + x1);
			write_com(0x8f - i + 4);	write_d(buf1[3] + x1);
			write_com(0x8f - i + 5);	write_d(buf1[4] + x1);
			write_com(0x8f - i + 6);	write_d(buf1[5] + x1);
		}
		if(key == 11){//复位
			write_com(0x01);
			i = 0;
			j = 0;
			flag1 = 0;
			flag_x = 0;
			count1 = 0;
			count2 = 0;
			count3 = 0;
			key = ' ';
			for(m = 0; m < 16; m++){
				buf1[m] = ' ';
				buf2[m] = ' ';
				buf3[m] = ' ';
			}
		}
		if(key == 12){//"="
			flag2 = 1;
		}
		if(key == 13){//"+"
			flag1 = 1;
		}
		if(key == 14){//"-"
			flag1 = 2;
		}
		if(key == 15){//"*"
			flag1 = 3;
		}
		if(key == 16){//"/"
			flag1 = 4;
		}
		if(flag1 != 0){
			if(flag_x == 0){
				flag_x = 1;
				write_com(0x01);
				if(key == 13){
					write_com(0xcf);	write_d("+");
					flag1 = 1;
				}
				if(key == 14){
					write_com(0xcf);	write_d("-");
					flag1 = 2;					
				}
				if(key == 15){
					write_com(0xcf);	write_d("*");
					flag1 = 3;					
				}
				if(key == 16){
					write_com(0xcf);	write_d("/");
					flag1 = 4;				
				}
				write_com(0x8f - j + 1); write_d(buf2[0] + x1);
				write_com(0x8f - j + 2); write_d(buf2[1] + x1);
				write_com(0x8f - j + 3); write_d(buf2[2] + x1);
				write_com(0x8f - j + 4); write_d(buf2[3] + x1);
				write_com(0x8f - j + 5); write_d(buf2[4] + x1);
				write_com(0x8f - j + 6); write_d(buf2[5] + x1);
			}
			conve();
			calculate();
		}
	}
}