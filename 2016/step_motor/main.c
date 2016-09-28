#include<regx52.h>
#include "key.h";
#include "led.h";


#define EVERY05MS 	500
#define EVERY2MS 	2000

#define REVERSE 	11	//B
#define FOREWARD 	10	//A
#define START	 14	//E
#define STOP 	 15	//F
#define ENTER	 13	//D
#define CLEAR	 12	//C

#define DISPLAY_N	0
#define CURRENT_N	1

//���**************************
//#define MOTOR_A P1_4
//#define MOTOR_B P1_6
//#define MOTOR_C P1_5
//#define MOTOR_D P1_7

//*********���������*****************
#define MOTOR_A P3_2
#define MOTOR_B P3_3
#define MOTOR_C P3_4
#define MOTOR_D P3_5
//***********************************

int j_m = 0, T_m;
char s_m = 3;


char is_speed_changed();
char speed_change();
void motor_run();
void motor_stop();
//**************************************


void system_init();
void timer0_init();
void timer1_init();
char* convert_to_str(char, int);
void key_handle(char);
void LED_clear();
void change_speed_in_main();


int j_ = 0, j_j = 6, current_n = 0, set_n = 0, dis_n = 0, key_pressed_time = 0;
char direction = 0, key_char, set_direction = 0, dis_direction, number_key_i  = 0,is_clear = 0, is_key_pressed = 0,
		display_now = CURRENT_N, is_stop_button = 0;
char str[] = "    ";
char key_show_n[] = {0, 0, 0};

//char code test[] = {
//	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
//};




void main(){
	welcome();
	
	system_init();
	
	while(1){
		
//		key_handle(key_char);
				
		change_speed_in_main();//����
	
	}
}

void system_init(){
	char set_value = 0;
	j_m = 0;
	T_m = 0;
	s_m = 3;
	j_ = 0;
	current_n = 0;
	set_n = 0;
	dis_n = 0;
	display_now = CURRENT_N;
	is_stop_button = 0;
	direction = 0;
	key_char = -1;
	set_direction = 0;
	dis_direction = 0;
	number_key_i = 0;
	is_clear = 0;
	is_key_pressed = 0;
	for(set_value = 0; set_value < 4; set_value++){
		str[set_value] = ' ';
		if(set_value != 3){
			key_show_n[set_value] = 0;
		}
	}
	
	LED_init();
	timer0_init();
	timer1_init();
}



//timer0���ڵ��ת��
void timer0() interrupt 1{
	TH0 = (65536 - EVERY05MS) / 256;
	TL0 = (65536 - EVERY05MS) % 256;

	motor_run();//���ת��

	
}
                
//timer1������ʾ�ͼ���ɨ��
void timer1() interrupt 3{
	TH1 = (65536 - EVERY2MS) / 256;
	TL1 = (65536 - EVERY2MS) % 256;
	
	
	key_char = get_key();//��ü�ֵ
	
//	display(convert_to_str(direction, current_n_dis));
	
	key_handle(key_char);//���ݼ�ִֵ����Ӧ����


}


//ͨ���ı�j_j ���ñ����ٶȣ����ٶȣ�
void change_speed_in_main(){
	j_++;
	if(is_speed_changed() == 1){
		if(j_ % j_j == 0){
			speed_change();
//			if(current_n >= 0){
//				current_n_dis = current_n;
//			}
		}
	}
}

//��������
void key_handle(char key_now){
	
	
	if(key_now < 0 && is_key_pressed == 0 && is_stop_button == 0){//key_now < 0,ɨ��ʱû�а������£�is_key_pressed == 0,֮ǰû�а������£�
		if(display_now == DISPLAY_N){//�ղ�����ʾ�����״̬����һ������Ȼ������ʾ��ǰת��״̬
			display("    ");
			display_now = CURRENT_N;
		}
		display(convert_to_str(direction, current_n));//��ʾ��ǰ�����ٶ�
		return;//û�а���
	}
	
	is_key_pressed = 1;//�а������£���ʾ�ı�
	
	if(key_now >= 0 && key_now <= 9){//���ּ�
		is_clear = 0;
		if(number_key_i >= 3) return;//������3λ��
		key_show_n[number_key_i] = key_now;		//�����ַ�������
		//�������������ֵ
		dis_n = key_show_n[(number_key_i == 2) ? (number_key_i - 2) : 2] * 100 + key_show_n[(number_key_i > 0) ? (number_key_i - 1) : 1] * 10 
			+ key_show_n[number_key_i];

		number_key_i++;
	}
	
	
	else if(key_now == START){//����
		
		system_init();
		
//		is_stop_button = 0;
//		is_clear = 0;
		
//		is_key_pressed = 0;
		
		//����ʱ�ٶ�Ϊ0
//		set_n = 0;
//		dis_n = 0;
//		current_n = 0;
		
//		TR0 = 1;
//		TR1 = 1;
	}
	
	
	else if(key_now == STOP){//ֹͣ
		is_stop_button = 1;
		is_clear = 0;
		//�ص��
		motor_stop();
		//����ʾ
		LED_stop();
		//����Ķ�ʱ��ֹͣ
		TR0 = 0;
//		TR1 = 0;
	}
	
	
	else if(key_now == CLEAR){//����
		is_clear = 1;
		
		//ȥ��֮ǰ���ٶ�������Ϣ
		number_key_i = 0;
		key_show_n[0] = 0;
		key_show_n[1] = 0;
		key_show_n[2] = 0;
	}
	
	
	else if(key_now == FOREWARD){//��ת
		is_clear = 0;
		//����ʾ����������enter�ٸı�ת��
		dis_direction = 0;
	}
	
	
	else if(key_now == REVERSE){//��ת
		is_clear = 0;
		//����ʾ����������enter�ٸı�ת��
		dis_direction = 1;
	}
	
	
	else if(key_now == ENTER){//�س�
		
		//������������
		is_clear = 0;
		number_key_i = 0;
		key_show_n[0] = 0;
		key_show_n[1] = 0;
		key_show_n[2] = 0;
		
		if(dis_n <= 240){//����ֵ��Ч
			set_n = dis_n;
			set_direction = dis_direction;
		}

		
		is_key_pressed = 0;//��������
		
	}
	
	
	//�������µ���ʾ
	if(is_clear == 1 && is_stop_button == 0){
		display("    ");
	}
	
	else if(is_stop_button == 0 && is_clear == 0){
		//ֻ��ʾһ��ʱ�䣬�������ʱ��û�а����ͻص���ʾ�ٶ�
		key_pressed_time++;
		if(key_pressed_time >= 5000){// 5000 * 2ms
			is_key_pressed = 0;
		}
		
		if(display_now == CURRENT_N){
			display("    ");
			display_now = DISPLAY_N;
		}
		//�������240����ʾ
		if(dis_n > 240){
			display("ERRO");
		}
		//��ʾ������Ϣ
		else{
			display(convert_to_str(dis_direction, dis_n));
		}

	}
}


//******************************************���**********************************************************
//�ı����ٶ�
char speed_change(){
	
//	if(reset_n){
//		if(current_n != 0 && set_n != 0){
//			current_t_over_4 = 750 * 2 / current_n;
//			set_t_over_4 = 750 * 2 / set_n;
//		}
//		reset_n = 0;
//	}
		
	//�ٶȺͷ��򶼲���
	if(current_n == set_n && direction == set_direction){		
		
		return -1;
	}
	//���״̬ʲôʱ��ᷢ����������
	if(current_n == 0 && set_n == 0){
		P1 = 0XF0; //ͣ���
		current_n = 0;
		return 0;
	}
	//����ת��Ϊ0
	else if(current_n != 0 && set_n == 0){
		P1 = 0XF0; //ͣ���
		current_n = 0;
		return 0;
	}
	else{
//		if(current_n == 0){
//			current_n = 1;
//		}

		//���򲻱���ٶ�
		if(direction == set_direction){
//			
//			if(current_t_over_4 < set_t_over_4){
//				current_t_over_4++;
//				
//			}
//			else{
//				current_t_over_4--;
//				
//			}
			
			if(current_n < set_n){
				current_n++;
			}
			else{
				current_n--;
			}	
			
		}
		

		//����ı���ٶ�
		else if(direction != set_direction){
//			
//			if(current_t_over_4 < 1500){
//				current_t_over_4++;
//			}
//			//��1תÿ�־�ͣ��
//			else if(current_t_over_4 == 1500){
//				P1 = 0XF0; //ͣ���
//				//��ʱ�ȴ����ֹͣ 
//				while(i_m--);
//				direction = set_direction;
//			}
			//���ٵ�0�ٱ䷽��Ȼ�����ͬ��ͬ�ٵ�������
			current_n--;
			if(current_n == 0){
				direction = set_direction;
			}
			
		}
//		current_n = 750 * 2 / current_t_over_4;

		return 1;
	}
}

//�����ٶȻ�����û�б仯
char is_speed_changed(){
		//�ٶȺͷ��򶼲���
	if(current_n == set_n && direction == set_direction){
		return 0;
	}
	return 1;
}


//���ת���ĺ���
//��ʱ�ж��У�0.5ms����һ�δ˺����� ÿT * 0.5ms״̬�ı�һ��

void motor_run(){
	//0ת���ǲ�ת��
	if(current_n == 0) return;
	
	//0.5ms�����⺯��һ�Σ��ٶȲ���0������£���ʱ
	j_m++;
	
	//�����ٶ��������,���Ҫ������ǲ��������
	T_m = 750 * 2 / current_n;
	if(j_m % T_m != 0){
		return;
	}
	
	//���� T_m * 0.5ms �ı�һ�ε��״̬
	if(direction == 0){
		s_m++;
	}
	else if(direction == 1){
		s_m--;
	}
	switch(s_m){
		case 1:
			MOTOR_D = 0;
			MOTOR_B = 1;
			if(direction == 1){
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
			if(direction == 0){
				s_m = 0;
			}
			break;


//		case 1:
//			MOTOR_A = 1;
//			MOTOR_D = 0;
//			if(direction == 1){
//				s_m = 5;
//			}
//			break;
//		case 2:
//			MOTOR_B = 1;
//			MOTOR_A = 0;
//			break;
//		case 3:
//			MOTOR_C = 1;
//			MOTOR_B = 0;
//			break;
//		case 4:
//			MOTOR_D = 1;
//			MOTOR_C = 0;
//			if(direction == 0){
//				s_m = 0;
//			}
//			break;
	}
}

//ͣ���
void motor_stop(){
	P1 = 0XF0; //ͣ���
	current_n = 0;
}
//****************************************************�����ǵ��**********************************************


//**************************���ȶ�**************************************




//���� ת��ת��Ϊ�ַ����������
char* convert_to_str(char _direction, int _n){
	
	char _i_s = 0;
	char _rst_str;
	for(_rst_str = 0; _rst_str < 4; _rst_str++){
		str[_rst_str] = ' ';
	}
	
//	if(_n > 240){
//		return "ERRO";	
//	}
	if(_direction == 0){
		str[0] = 'D';
	}
	else{
		str[0] = 'C';
	}
	if(_n == 0){
		str[3] = '0';
	}
	else{
		while(_n){
			str[3 - _i_s] = (char)(_n % 10) + '0';
			_n = _n / 10;
			_i_s++;
		}
	}
	
	return str;
}


void timer0_init(){
	TMOD = 0X11;//��ʱ��0������ʽ1
	TH0 = (65536 - EVERY05MS) / 256;
	TL0 = (65536 - EVERY05MS) % 256;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void timer1_init(){
//	TMOD = 0X10;//��ʱ��1������ʽ1
	TH1 = (65536 - EVERY2MS) / 256;
	TL1 = (65536 - EVERY2MS) % 256;
	ET1 = 1;
	EA = 1;
	TR1 = 1;
}
