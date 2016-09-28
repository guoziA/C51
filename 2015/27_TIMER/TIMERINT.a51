

	OUTBIT	EQU	0E101H
	CLK164	EQU	0E102H
	DAT164	EQU	0E102H
	FLAG	EQU	20H.0
	
	LEDBUF	EQU	60H
	

	ORG	0000H
	LJMP	MAIN	;主程序
	ORG	0BH
	LJMP	TIMER0INTERUPT	;定时器0用于计数
											;ORG	01BH
											;LJMP	TIMER1INTERUPT	;定时器1用于显示
	
	ORG	0100H
MAIN:
	SETB	FLAG	
	MOV	R5,	#00H	;清空计数
	MOV	TMOD,	#01H	;定时器0用模式1
	
	MOV	TH0,	#3CH	;初值15536
	MOV	TL0,	#0B0H
	
											;MOV	TH1,	#0C3H	;初值50000
											;MOV	TL1,	#50H
	
	SETB	EA	;允许总中断
	SETB	ET0	;定时器0允许中断
											;SETB	ET1	;定时器1允许中断
	SETB	TR0	;开定时器0
											;CLR	TR1	;定时器1先不开
	
LOOPM:	
	CJNE	R5,	#14H,	CHECKFLAG;检查flag						$	;R5用于计数，当R5=14H改变状态
	MOV	R5,	#00H	;计数器清零
CHECKFLAG:
	JB	FLAG,	CALLDIS	;跳转到点亮
	LCALL	LEDOFF	;否则跳转到熄灭
	LJMP	LOOPM
	
CALLDIS:
	LCALL	DISPLAYLED;点亮数码管							SETB	TR1	;开定时器1，点亮数码管
	LJMP	LOOPM
	
TIMER0INTERUPT:	;定时器0用于计数
	MOV	TH0,	#3CH	;重装初值
	MOV	TL0,	#0B0H
	INC	R5	;计数器+1
	RETI
	
											;TIMER1INTERUPT:	;定时器1用于显示
												;MOV	TH1,	#0C3H	;重装初值
												;MOV	TL1,	#50H
												;CLR	TR1	;关定时器1
												;LCALL	DISPLAYLED	;显示
												;SETB	TR1	;开定时器
												;RETI
	
LEDOFF:		;熄灭全部数码管
											;CLR	TR1	;关定时器1停止刷新显示
	MOV	A,	#00H	
	MOV	DPTR,	#OUTBIT
	MOVX	@ DPTR,	A	;关全部数码管
	RET
	
DISPLAYLED:	;数码管显示函数
	MOV	R0,	#LEDBUF	;取缓冲器第一个数到R0
	MOV	R1,	#6	;6位数码管
	MOV	R2,	#20H	;第一位数码管0010000
LOOP:
	MOV	DPTR,	#OUTBIT	;关显示
	MOV	A,	#00H
	MOVX	@ DPTR,	A
	MOV	A,	@ R0
	
	;显示码送164
	MOV	B,	#8	;8段
DLP:	
	;取C
	RLC	A
	MOV	R3,	A
	MOV	ACC.0,	C
	
	;屏蔽时钟位
	ANL	A,	#0FDH
	MOV	DPTR,	#DAT164	;送数据
	MOVX	@ DPTR,	A
	
	;给时钟位一个脉冲，锁存1段数据
	MOV	DPTR, 	#CLK164
	ORL	A,	#02H
	MOVX	@ DPTR,	A
	ANL	A,	#0FDH
	MOVX	@ DPTR,	A
	
	;取回R3
	MOV	A,	R3
	DJNZ	B,	DLP	;共锁存8段数据
	
	MOV	DPTR,	#OUTBIT	;显示1位数码管
	MOV	A,	R2
	MOVX	@ DPTR,	A
	
	MOV	R6,	#0
	LCALL	DELAY
	
	MOV	A,	R2	;显示下1位
	RR 	A
	MOV	R2,	A
	
	INC	R0
	
	DJNZ	R1,	LOOP	;共6位
	
	MOV	DPTR,	#OUTBIT	;关显示
	MOV	A,	#0
	MOVX	@ DPTR,	A
	RET
	
DELAY:	;延时函数
	MOV	R7,	#64H	;100，如果发现数码管一闪一闪的，减小这个数试试
DELAYLOOP:
	DJNZ	R7,	DELAYLOOP
	DJNZ	R6,	DELAYLOOP
	RET
	
	ORG	60H	;显示码表	abcdef
	DB	5FH,	7CH,	58H,	5EH,	7BH,	71H
	
;FILLBUF:
	;MOV	R0,	#LEDBUF
	;MOV	A,	#1
	;MOV	B,	A
;FILLLP:
	;MOV	DPTR,	#LEDMAP
	;MOVC	A,	@ DPTR+A
	;MOV	@R0,	A
	;INC	B
	;MOV	A,	B
	;CJNE	R0,	#LEDBUF+6,	FILLLP
	;RET
	
	END
