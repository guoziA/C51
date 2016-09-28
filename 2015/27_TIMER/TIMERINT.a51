

	OUTBIT	EQU	0E101H
	CLK164	EQU	0E102H
	DAT164	EQU	0E102H
	FLAG	EQU	20H.0
	
	LEDBUF	EQU	60H
	

	ORG	0000H
	LJMP	MAIN	;������
	ORG	0BH
	LJMP	TIMER0INTERUPT	;��ʱ��0���ڼ���
											;ORG	01BH
											;LJMP	TIMER1INTERUPT	;��ʱ��1������ʾ
	
	ORG	0100H
MAIN:
	SETB	FLAG	
	MOV	R5,	#00H	;��ռ���
	MOV	TMOD,	#01H	;��ʱ��0��ģʽ1
	
	MOV	TH0,	#3CH	;��ֵ15536
	MOV	TL0,	#0B0H
	
											;MOV	TH1,	#0C3H	;��ֵ50000
											;MOV	TL1,	#50H
	
	SETB	EA	;�������ж�
	SETB	ET0	;��ʱ��0�����ж�
											;SETB	ET1	;��ʱ��1�����ж�
	SETB	TR0	;����ʱ��0
											;CLR	TR1	;��ʱ��1�Ȳ���
	
LOOPM:	
	CJNE	R5,	#14H,	CHECKFLAG;���flag						$	;R5���ڼ�������R5=14H�ı�״̬
	MOV	R5,	#00H	;����������
CHECKFLAG:
	JB	FLAG,	CALLDIS	;��ת������
	LCALL	LEDOFF	;������ת��Ϩ��
	LJMP	LOOPM
	
CALLDIS:
	LCALL	DISPLAYLED;���������							SETB	TR1	;����ʱ��1�����������
	LJMP	LOOPM
	
TIMER0INTERUPT:	;��ʱ��0���ڼ���
	MOV	TH0,	#3CH	;��װ��ֵ
	MOV	TL0,	#0B0H
	INC	R5	;������+1
	RETI
	
											;TIMER1INTERUPT:	;��ʱ��1������ʾ
												;MOV	TH1,	#0C3H	;��װ��ֵ
												;MOV	TL1,	#50H
												;CLR	TR1	;�ض�ʱ��1
												;LCALL	DISPLAYLED	;��ʾ
												;SETB	TR1	;����ʱ��
												;RETI
	
LEDOFF:		;Ϩ��ȫ�������
											;CLR	TR1	;�ض�ʱ��1ֹͣˢ����ʾ
	MOV	A,	#00H	
	MOV	DPTR,	#OUTBIT
	MOVX	@ DPTR,	A	;��ȫ�������
	RET
	
DISPLAYLED:	;�������ʾ����
	MOV	R0,	#LEDBUF	;ȡ��������һ������R0
	MOV	R1,	#6	;6λ�����
	MOV	R2,	#20H	;��һλ�����0010000
LOOP:
	MOV	DPTR,	#OUTBIT	;����ʾ
	MOV	A,	#00H
	MOVX	@ DPTR,	A
	MOV	A,	@ R0
	
	;��ʾ����164
	MOV	B,	#8	;8��
DLP:	
	;ȡC
	RLC	A
	MOV	R3,	A
	MOV	ACC.0,	C
	
	;����ʱ��λ
	ANL	A,	#0FDH
	MOV	DPTR,	#DAT164	;������
	MOVX	@ DPTR,	A
	
	;��ʱ��λһ�����壬����1������
	MOV	DPTR, 	#CLK164
	ORL	A,	#02H
	MOVX	@ DPTR,	A
	ANL	A,	#0FDH
	MOVX	@ DPTR,	A
	
	;ȡ��R3
	MOV	A,	R3
	DJNZ	B,	DLP	;������8������
	
	MOV	DPTR,	#OUTBIT	;��ʾ1λ�����
	MOV	A,	R2
	MOVX	@ DPTR,	A
	
	MOV	R6,	#0
	LCALL	DELAY
	
	MOV	A,	R2	;��ʾ��1λ
	RR 	A
	MOV	R2,	A
	
	INC	R0
	
	DJNZ	R1,	LOOP	;��6λ
	
	MOV	DPTR,	#OUTBIT	;����ʾ
	MOV	A,	#0
	MOVX	@ DPTR,	A
	RET
	
DELAY:	;��ʱ����
	MOV	R7,	#64H	;100��������������һ��һ���ģ���С���������
DELAYLOOP:
	DJNZ	R7,	DELAYLOOP
	DJNZ	R6,	DELAYLOOP
	RET
	
	ORG	60H	;��ʾ���	abcdef
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
