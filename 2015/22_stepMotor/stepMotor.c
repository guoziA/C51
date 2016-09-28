#include<regx52.h>
#define step1 P3_2//??IO?????
#define step2 P3_3
#define step3 P3_4
#define step4 P3_5
#define signal P3_0
#define DCstep P1_7


void delayms(int xms){
	int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}
void main()
{
	
	unsigned int i = 0;
	unsigned char s = 0;
	DCstep = 0;
	while(1)
	{
		s++;
		switch(s){//??IO???
			case 1:
				step2 = 0;
				step4 = 0;
				step1 = 1;
				step3 = 0;
				break;
			case 2:
				step1 = 0;
				step3 = 0;
				step4 = 0;
				step2 = 1;
				break;
			case 3:
				step2 = 0;
				step4 = 0;
				step3 = 1;
				step1 = 0;
				break;
			case 4:
				step1 = 0;
				step3 = 0;
				step2 = 0;
				step4 = 1;
				s = 0;
				signal = !signal;
				DCstep = !DCstep;
				break;
		}
		delayms(200);
	}
}