#include "motor.h";
#include<regx52.h>

//n=60/��T*20��
//T = 3/n (s) = 3000/n (ms)
//T/4 = 750/n (ms)
//(T/4)max = 750/240 = 3.125 (ms)
//�����ж�ʱ��Ϊ 0.5 ms
//���� 0 ��ʾ��D�� 1 ��ʾ��C


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
	
	//�������ٶ�
//	sc_c++;
//	if(sc_c % 20 != 0){
//		return -1;
//	}
//	sc_c = 0;
		
	//�ٶȺͷ��򶼲���
	if(current_v == set_v && current_d == set_d){
		return -1;
	}

	if(current_v == 0 && set_v == 0){
		P1 = 0XF0; //ͣ���
		return 0;
	}
	else if(current_v != 0 && set_v == 0){
		P1 = 0XF0; //ͣ���
		return 0;
	}
	else{
		if(current_v == 0){
			current_v = 1;
		}
		current_t_over_4 = 750 * 2 / current_v;
		set_t_over_4 = 750 * 2 / set_v;

		//���򲻱���ٶ�
		if(current_d == set_d){
			if(current_t_over_4 < set_t_over_4){
				current_t_over_4++;
			}
			else{
				current_t_over_4--;
				
			}
		}
		//����ı���ٶ�
		else if(current_d != set_d){
			
			if(current_t_over_4 < 1500){
				current_t_over_4++;
			}
			//��1תÿ�־�ͣ��
			else if(current_t_over_4 == 1500){
				P1 = 0XF0; //ͣ���
				//��ʱ�ȴ����ֹͣ 
				while(i_m--);
				current_d = set_d;
			}
		}
		return 750 * 2 / current_t_over_4;
	}
}


char is_speed_changed(int current_v, char current_d, int set_v, char set_d){
		//�ٶȺͷ��򶼲���
	if(current_v == set_v && current_d == set_d){
		return 0;
	}
	return 1;
}

//��ʱ�ж��У�0.5ms����һ�δ˺����� ÿT * 0.5ms״̬�ı�һ��


void motor_run(int current_v, char current_d){
	
	j_m++;
	
	T_m = 750 * 2 / current_v;
	if(j_m % T_m != 0){
		return;
	}
	
	//�ı���״̬
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



