#include<regx52.h>

#define KEY_0 P1_0//按键IO口
#define KEY_1 P1_2
#define KEY_2 P1_4
#define KEY_3 P1_6

#define LED_0 P0_0//LED的IO口
#define LED_1 P0_1
#define LED_2 P0_2
#define LED_3 P0_3

void delayms(int xms){
	int i, j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void Light(){
	if(KEY_0 == 0){
		LED_0 = !LED_0;
		delayms(500);
		
	}else if(KEY_1 == 0){
		LED_1 = !LED_1;
		delayms(500);
		
	}else if(KEY_2 == 0){
		LED_2 = !LED_2;
		delayms(500);
		
	}else if(KEY_3 == 0){
		LED_3 = !LED_3;
		delayms(500);
	}
}

void main(){
	while(1){
		Light();
	}
}