 #include <reg52.h> 

//sfr  P4  = 0xE8;
sbit   speaker = P1^0; 

unsigned char timer0h, timer0l, time; 

code unsigned char FREQH[] = { //高八度

    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //低1234567

    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,//1,2,3,4,5,6,7,i 

    0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //高234567 

    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};   //超高1234567 



code unsigned char FREQL[] = { //低八度

    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //低1234567 

    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //1,2,3,4,5,6,7,i 

    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,             //高234567 

    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //超高1234567 

//-------------------------------------- 

//世上只有妈妈好

code unsigned char sszymmh[] = { 

6, 2, 3, 5, 2, 1, 3, 2, 2, 5, 2, 2, 1, 3, 2, 6, 2, 1, 5, 2, 1,  

6, 2, 4, 3, 2, 2, 5, 2, 1, 6, 2, 1, 5, 2, 2, 3, 2, 2, 1, 2, 1, 

6, 1, 1, 5, 2, 1, 3, 2, 1, 2, 2, 4, 2, 2, 3, 3, 2, 1, 5, 2, 2, 

5, 2, 1, 6, 2, 1, 3, 2, 2, 2, 2, 2, 1, 2, 4, 5, 2, 3, 3, 2, 1, 

2, 2, 1, 1, 2, 1, 6, 1, 1, 1, 2, 1, 5, 1, 6, 0, 0, 0}; 

//-------------------------------------- 

//test
code unsigned char test[] = {
	1,0,3,2,0,3,3,0,3,4,0,3,5,0,3,6,0,3,7,0,3,
	1,1,3,2,1,3,3,1,3,4,1,3,5,1,3,6,1,3,7,1,3,
	1,2,3,2,2,3,3,2,3,4,2,3,5,2,3,6,2,3,7,2,3,
	1,3,3,2,3,3,3,3,3,4,3,3,5,3,3,6,3,3,7,3,3
};

void t0int() interrupt 1       
 
{ 

TR0 = 0;                     

speaker = !speaker;           

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

     for(t2 = 0; t2 < 5000; t2++); //  for(t2 = 0; t2 < 8000; t2++);
 
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
void delayms(int xms){
	int i, time;
	for(i = xms; i > 0; i--)
			for(time = 110; time > 0; time--);
}
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

     k = sszymmh[i] + 7 * sszymmh[i + 1] - 1;   
     timer0h = FREQH[k];   
     timer0l = FREQL[k];    
     time = test[i + 2]; //时间，第三个数
     i += 3; 
		song();      

     } 

} 

} 