#include "motor.h";
#include<regx52.h>

//n=60/（T*20）
//T = 3/n (s) = 3000/n (ms)
//T/4 = 750/n (ms)
//(T/4)max = 750/240 = 3.125 (ms)
//设置中断时间为 0.5 ms
//方向 0 表示正D， 1 表示反C


//p14 - motor_a
//p15 - motor_b
//p16 - motor_c
//p17 - motor_d

#define MOTOR_A P1_4
#define MOTOR_B P1_5
#define MOTOR_C P1_6
#define MOTOR_D P1_7

int current_t_over_4, set_t_over_4, j_m = 0, T_m;
char i_m = 100, s_m = 1, sc_c = 0;

char speed_change(int current_v, char current_d, int set_v, char set_d){
	
	//减慢加速度
//	sc_c++;
//	if(sc_c % 20 != 0){
//		return -1;
//	}
//	sc_c = 0;
		
	//速度和方向都不变
	if(current_v == set_v && current_d == set_d){
		return -1;
	}

	if(current_v == 0 && set_v == 0){
		P1 = 0XF0; //停电机
		return 0;
	}
	else if(current_v != 0 && set_v == 0){
		P1 = 0XF0; //停电机
		return 0;
	}
	else{
		if(current_v == 0){
			current_v = 1;
		}
		current_t_over_4 = 750 * 2 / current_v;
		set_t_over_4 = 750 * 2 / set_v;

		//方向不变变速度
		if(current_d == set_d){
			if(current_t_over_4 < set_t_over_4){
				current_t_over_4++;
			}
			else{
				current_t_over_4--;
				
			}
		}
		//方向改变变速度
		else if(current_d != set_d){
			
			if(current_t_over_4 < 1500){
				current_t_over_4++;
			}
			//到1转每分就停机
			else if(current_t_over_4 == 1500){
				P1 = 0XF0; //停电机
				//延时等待电机停止 
				while(i_m--);
				current_d = set_d;
			}
		}
		return 750 * 2 / current_t_over_4;
	}
}


char is_speed_changed(int current_v, char current_d, int set_v, char set_d){
		//速度和方向都不变
	if(current_v == set_v && current_d == set_d){
		return 0;
	}
	return 1;
}

//定时中断中，0.5ms进入一次此函数， 每T * 0.5ms状态改变一次


void motor_run(int current_v, char current_d){
	
	j_m++;
	
	T_m = 750 * 2 / current_v;
	if(j_m % T_m != 0){
		return;
	}
	
	//改变电机状态
	if(current_d == 0){
		s_m++;
	}
	else if(current_d == 1){
		s_m--;
	}
	switch(s_m){
		case 1:
			MOTOR_D = 0;
			MOTOR_B = 1;
			if(current_d == 1){
				s_m = 5;
			}
			break;
		case 2:
			MOTOR_C = 0;
			MOTOR_A = 1;
			break;
		case 3:
			MOTOR_B = 0;
			MOTOR_D = 1;
			break;
		case 4:
			MOTOR_A = 0;
			MOTOR_C = 1;
			if(current_d == 0){
				s_m = 0;
			}
			break;
	}
}



