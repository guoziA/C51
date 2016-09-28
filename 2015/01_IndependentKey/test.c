#include "global.h"
#include "other.h"

extern char GLOBAL_CHAR;
extern idata char test_char1;
extern idata char test_char3;

void fun1(){
	GLOBAL_CHAR = 1;
	test_char3 = 1;
	
	test_char4 = &test_char3;
	
	GLOBAL_CHAR = *test_char4;
}

char func2(){
	return 1;
}

char func3(char i){
	i = 1;
	func4(1);
	return i;
}