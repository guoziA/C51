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

//电机**************************
//#define MOTOR_A P1_4
//#define MOTOR_B P1_6
//#define MOTOR_C P1_5
//#define MOTOR_D P1_7

//*********开发板调试*****************
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
				
		change_speed_in_main();//变速
	
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



//timer0用于电机转动
void timer0() interrupt 1{
	TH0 = (65536 - EVERY05MS) / 256;
	TL0 = (65536 - EVERY05MS) % 256;

	motor_run();//电机转动

	
}
                
//timer1用于显示和键盘扫描
void timer1() interrupt 3{
	TH1 = (65536 - EVERY2MS) / 256;
	TL1 = (65536 - EVERY2MS) % 256;
	
	
	key_char = get_key();//获得键值
	
//	display(convert_to_str(direction, current_n_dis));
	
	key_handle(key_char);//根据键值执行相应操作


}


//通过改变j_j 设置变速速度（加速度）
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

//按键处理
void key_handle(char key_now){
	
	
	if(key_now < 0 && is_key_pressed == 0 && is_stop_button == 0){//key_now < 0,扫描时没有按键按下；is_key_pressed == 0,之前没有按键按下；
		if(display_now == DISPLAY_N){//刚才是显示输入的状态，清一下屏，然后变成显示当前转速状态
			display("    ");
			display_now = CURRENT_N;
		}
		display(convert_to_str(direction, current_n));//显示当前方向，速度
		return;//没有按键
	}
	
	is_key_pressed = 1;//有按键按下，显示改变
	
	if(key_now >= 0 && key_now <= 9){//数字键
		is_clear = 0;
		if(number_key_i >= 3) return;//不大于3位数
		key_show_n[number_key_i] = key_now;		//把数字放入数组
		//计算输入的数的值
		dis_n = key_show_n[(number_key_i == 2) ? (number_key_i - 2) : 2] * 100 + key_show_n[(number_key_i > 0) ? (number_key_i - 1) : 1] * 10 
			+ key_show_n[number_key_i];

		number_key_i++;
	}
	
	
	else if(key_now == START){//启动
		
		system_init();
		
//		is_stop_button = 0;
//		is_clear = 0;
		
//		is_key_pressed = 0;
		
		//启动时速度为0
//		set_n = 0;
//		dis_n = 0;
//		current_n = 0;
		
//		TR0 = 1;
//		TR1 = 1;
	}
	
	
	else if(key_now == STOP){//停止
		is_stop_button = 1;
		is_clear = 0;
		//关电机
		motor_stop();
		//关显示
		LED_stop();
		//电机的定时器停止
		TR0 = 0;
//		TR1 = 0;
	}
	
	
	else if(key_now == CLEAR){//清屏
		is_clear = 1;
		
		//去掉之前的速度设置信息
		number_key_i = 0;
		key_show_n[0] = 0;
		key_show_n[1] = 0;
		key_show_n[2] = 0;
	}
	
	
	else if(key_now == FOREWARD){//正转
		is_clear = 0;
		//先显示出来，按下enter再改变转向
		dis_direction = 0;
	}
	
	
	else if(key_now == REVERSE){//反转
		is_clear = 0;
		//先显示出来，按下enter再改变转向
		dis_direction = 1;
	}
	
	
	else if(key_now == ENTER){//回车
		
		//清除输入的数组
		is_clear = 0;
		number_key_i = 0;
		key_show_n[0] = 0;
		key_show_n[1] = 0;
		key_show_n[2] = 0;
		
		if(dis_n <= 240){//输入值有效
			set_n = dis_n;
			set_direction = dis_direction;
		}

		
		is_key_pressed = 0;//按键结束
		
	}
	
	
	//清屏按下的显示
	if(is_clear == 1 && is_stop_button == 0){
		display("    ");
	}
	
	else if(is_stop_button == 0 && is_clear == 0){
		//只显示一段时间，过了这段时间没有按键就回到显示速度
		key_pressed_time++;
		if(key_pressed_time >= 5000){// 5000 * 2ms
			is_key_pressed = 0;
		}
		
		if(display_now == CURRENT_N){
			display("    ");
			display_now = DISPLAY_N;
		}
		//输入大于240的显示
		if(dis_n > 240){
			display("ERRO");
		}
		//显示输入信息
		else{
			display(convert_to_str(dis_direction, dis_n));
		}

	}
}


//******************************************电机**********************************************************
//改变电机速度
char speed_change(){
	
//	if(reset_n){
//		if(current_n != 0 && set_n != 0){
//			current_t_over_4 = 750 * 2 / current_n;
//			set_t_over_4 = 750 * 2 / set_n;
//		}
//		reset_n = 0;
//	}
		
	//速度和方向都不变
	if(current_n == set_n && direction == set_direction){		
		
		return -1;
	}
	//这个状态什么时候会发生？？？？
	if(current_n == 0 && set_n == 0){
		P1 = 0XF0; //停电机
		current_n = 0;
		return 0;
	}
	//设置转速为0
	else if(current_n != 0 && set_n == 0){
		P1 = 0XF0; //停电机
		current_n = 0;
		return 0;
	}
	else{
//		if(current_n == 0){
//			current_n = 1;
//		}

		//方向不变变速度
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
		

		//方向改变变速度
		else if(direction != set_direction){
//			
//			if(current_t_over_4 < 1500){
//				current_t_over_4++;
//			}
//			//到1转每分就停机
//			else if(current_t_over_4 == 1500){
//				P1 = 0XF0; //停电机
//				//延时等待电机停止 
//				while(i_m--);
//				direction = set_direction;
//			}
			//减速到0再变方向，然后就是同向不同速的问题了
			current_n--;
			if(current_n == 0){
				direction = set_direction;
			}
			
		}
//		current_n = 750 * 2 / current_t_over_4;

		return 1;
	}
}

//看看速度或方向有没有变化
char is_speed_changed(){
		//速度和方向都不变
	if(current_n == set_n && direction == set_direction){
		return 0;
	}
	return 1;
}


//电机转动的函数
//定时中断中，0.5ms进入一次此函数， 每T * 0.5ms状态改变一次

void motor_run(){
	//0转就是不转咯
	if(current_n == 0) return;
	
	//0.5ms进来这函数一次，速度不是0的情况下，计时
	j_m++;
	
	//根据速度算出周期,这个要检查下是不是这个数
	T_m = 750 * 2 / current_n;
	if(j_m % T_m != 0){
		return;
	}
	
	//经过 T_m * 0.5ms 改变一次电机状态
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

//停电机
void motor_stop(){
	P1 = 0XF0; //停电机
	current_n = 0;
}
//****************************************************以上是电机**********************************************


//**************************较稳定**************************************




//方向 转速转变为字符串用于输出
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
	TMOD = 0X11;//定时器0工作方式1
	TH0 = (65536 - EVERY05MS) / 256;
	TL0 = (65536 - EVERY05MS) % 256;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void timer1_init(){
//	TMOD = 0X10;//定时器1工作方式1
	TH1 = (65536 - EVERY2MS) / 256;
	TL1 = (65536 - EVERY2MS) % 256;
	ET1 = 1;
	EA = 1;
	TR1 = 1;
}
