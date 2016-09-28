#include<regx52.h>

sfr P4 = 0xE8;
sbit BUZZER = P4^4;


unsigned char timer0h, timer0l, time; 

code unsigned char FREQH[] = { //???

    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //?1234567

    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,//1,2,3,4,5,6,7,i 

    0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //?234567 

    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};   //??1234567 



code unsigned char FREQL[] = { //???

    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //?1234567 

    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //1,2,3,4,5,6,7,i 

    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,             //?234567 

    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //??1234567 


//test
code unsigned char thank_you[] = {
	0,0,0,5,0,1,5,0,1,3,1,2,0,0,1,3,1,1,4,1,1,3,1,1,0,1,1,7,0,1,2,1,1,1,1,1,7,0,1,1,1,1,1,1,2
};

void t0int() interrupt 1       
 
{ 

TR0 = 0;                     

BUZZER = !BUZZER;           

TH0 = timer0h;           
 
TL0 = timer0l; 

TR0 = 1;            

} 

//-------------------------------------- 

void delay(unsigned char t)    
 
{ 

unsigned char t1; 

unsigned long t2; 

for(t1 = 0; t1 < t; t1++)    

     for(t2 = 0; t2 < 8000; t2++); 
 
TR0 = 0;                     
 
} 

//-------------------------------------- 

void song()                
 
{ 

TH0 = timer0h;               
 
TL0 = timer0l; 

TR0 = 1;             

delay(time);                 

} 

//-------------------------------------- 

void main(void) 

{ 

unsigned char k, i; 

TMOD = 1;            

ET0 = 1;                

EA = 1;                  

while(1) { 

    i = 0; 

    time = 1;  

    while(1) { 

      k = thank_you[i] + 7 * thank_you[i + 1] - 1;   
 
      timer0h = FREQH[k];   

      timer0l = FREQL[k];    

      time = thank_you[i + 2]; //??,????

      i += 3; 

      song();            

     } 

} 

} 

