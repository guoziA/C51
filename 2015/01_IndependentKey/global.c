#include "global.h"

char GLOBAL_CHAR;
idata char test_char1;
idata char test_char2;

void func4(char r) reentrant{
	GLOBAL_CHAR = 1;
	test_char1 = 1;
}