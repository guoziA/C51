#include "key.h";
#include<regx52.h>

#define COL_0 P2_0
#define COL_1 P2_1
#define COL_2 P2_2
#define COL_3 P2_3
#define ROW_0 P2_7
#define ROW_1 P2_6
#define ROW_2 P2_5
#define ROW_3 P2_4



char key_now_ = -1;


char get_key(){
	//************开发板调试**************************************
	P2 = 0XFF;
	COL_0= 0;
	if(ROW_0 == 0){
		if(key_now_ != -1){//扫描完一次才能把key_now_ 从新置-1，一直按键不放只能返回一次正确的键值。
			return -1;
		}
		key_now_ = 14;
		return 14;
	}else if(ROW_1 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 0;
		return 0;
	}else if(ROW_2 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 15;
		return 15;
	}else if(ROW_3 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 13;
		return 13;
	}
	
	
	P2 = 0xff;
	COL_1 = 0;
	if(ROW_0 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 7;
		return 7;
	}else if(ROW_1 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 8;
		return 8;
	}else if(ROW_2 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 9;
		return 9;
	}else if(ROW_3 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 12;
		return 12;
	}
	
	
	P2 = 0xff;
	COL_2 = 0;
	if(ROW_0 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 4;
		return 4;
	}else if(ROW_1 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 5;
		return 5;
	}else if(ROW_2 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 6;
		return 6;
	}else if(ROW_3 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 11;
		return 11;
	}
	
	
	P2 = 0xff;
	COL_3 = 0;
	if(ROW_0 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 1;
		return 1;
	}else if(ROW_1 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 2;
		return 2;
	}else if(ROW_2 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 3;
		return 3;
	}else if(ROW_3 == 0){
		if(key_now_ != -1){
			return -1;
		}
		key_now_ = 10;
		return 10;
	}
	
	key_now_ = -1;
	
	return -1;


//**********************************************
//	P2 = 0XFF;
//	COL_0= 0;
//	if(ROW_0 == 0){
//		if(key_now_ != -1){//扫描完一次才能把key_now_ 从新置-1，一直按键不放只能返回一次正确的键值。
//			return -1;
//		}
//		key_now_ = 13;
//		return 13;
//	}else if(ROW_1 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 15;
//		return 15;
//	}else if(ROW_2 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 14;
//		return 0;
//	}else if(ROW_3 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 14;
//		return 14;
//	}
//	
//	
//	P2 = 0xff;
//	COL_1 = 0;
//	if(ROW_0 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 12;
//		return 12;
//	}else if(ROW_1 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 9;
//		return 9;
//	}else if(ROW_2 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 8;
//		return 8;
//	}else if(ROW_3 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 7;
//		return 7;
//	}
//	
//	
//	P2 = 0xff;
//	COL_2 = 0;
//	if(ROW_0 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 11;
//		return 11;
//	}else if(ROW_1 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 6;
//		return 6;
//	}else if(ROW_2 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 5;
//		return 5;
//	}else if(ROW_3 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 4;
//		return 4;
//	}
//	
//	
//	P2 = 0xff;
//	COL_3 = 0;
//	if(ROW_0 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 10;
//		return 10;
//	}else if(ROW_1 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 3;
//		return 3;
//	}else if(ROW_2 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 2;
//		return 2;
//	}else if(ROW_3 == 0){
//		if(key_now_ != -1){
//			return -1;
//		}
//		key_now_ = 1;
//		return 1;
//	}
//	key_now_ = -1;
//	return -1;
	
}