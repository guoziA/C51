#include<reg52.h>
#include<1602.h>

#define uint unsigned int 
#define uchar unsigned char

sbit rs = P1^0;
sbit rw = P1^1;
sbit e = P1^5;
void delay_ms(uint i);
void write_com(uint com);
void write_d(uchar d);
void init_1602();