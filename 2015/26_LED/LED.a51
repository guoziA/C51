				;�ҵ�ע�Ͷ��Ǹ�����⣬��Ҫ���Һ�˵�˵�
	LEDS	EQU	P0
	
	ORG	0000H			;PC��λ��0000H
		
	MOV 	A , 	#0FEH		;������F8H�����ۼ���A
LOOP:	MOV	LEDS,	A		;���ۼ���A������F8H�ŵ�P0 IO��
	LCALL	DELAY			;������ʱ�Ӻ���
	RL 	A			;A�е��������ƣ�Ӧ��������
	SJMP	LOOP			;��ת��LOOP��
	
	
DELAY:	MOV	R1,	#0FFH		;��ʱ������������FFH����R1
LP:	MOV 	R2,	#80H		;������80H����R2
LP1:	NOP				;��ʱһ��ʱ�䣬�����ò��ǵ���
	NOP							
	DJNZ	R2,	LP1		;R2�������Լ�1�������Ϊ0����ת��LP1
	DJNZ 	R1,	LP		;R1�������Լ�1�������Ϊ0����ת��LP
	RET				;�Ӻ������� return
	END				;���������־				