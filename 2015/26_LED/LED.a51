				;我的注释都是个人理解，不要听我胡说八道
	LEDS	EQU	P0
	
	ORG	0000H			;PC复位到0000H
		
	MOV 	A , 	#0FEH		;将数据F8H放入累加器A
LOOP:	MOV	LEDS,	A		;将累加器A的数据F8H放到P0 IO口
	LCALL	DELAY			;调用延时子函数
	RL 	A			;A中的数据左移，应该是左移
	SJMP	LOOP			;跳转到LOOP行
	
	
DELAY:	MOV	R1,	#0FFH		;延时函数，将数据FFH放入R1
LP:	MOV 	R2,	#80H		;将数据80H放入R2
LP1:	NOP				;延时一段时间，具体多久不记得了
	NOP							
	DJNZ	R2,	LP1		;R2中数据自减1，如果不为0，跳转到LP1
	DJNZ 	R1,	LP		;R1中数据自减1，如果不为0，跳转到LP
	RET				;子函数返回 return
	END				;程序结束标志				