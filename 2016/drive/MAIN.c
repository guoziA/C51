#include "driveincludes.h"
#include <math.h>

char *ItoC(uint);

void main(){
	uint i = 0, flag = 0, dly = 0;
	float temp = 0;
	drive_init();
	while(1){
		if(i % 500){
			if(flag){
				temp = DS18B20_GetT();
				//SERIAL_Print("temperature in main: ");
				//SERIAL_Print(ItoC(temp));
				flag = 0;
			}else{
				DS18B20_ConvertT();
				flag = 1;
			}
		}
		LED_Display((uint)temp);
		i++;
	}
}

char *ItoC(uint num){
	uchar i = 0;
	uint temp = num, level = 0;
	char str[20];
	for(;temp > 0; temp /= 10){
		i++;
	}
	level = (i == 1 || i == 2) ? pow(10, i - 1) : pow(10, i - 1) + 1;			/*为什么pow(10, 2) = 99	double 转换为int！！ */
	for(i = 0; level > 0; level /= 10){
		str[i] = num / level + 48;
		num %= level;
		i++;
	}
	return str;
}