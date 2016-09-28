
/***********************************************************************************************************/	    
		     #include <AT89x51.H>		
			 #include <intrins.h>
			 #define  RX  P3_1
			 #define  TX  P3_2
			 unsigned int  time=0;
			 unsigned int  timer=0;
			 unsigned char posit=0;
			 unsigned long S=0;
			 bit      flag =0;
			 unsigned char const discode[] ={ 0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef/*-*/};//����0~9
			 unsigned char const positon[3]={ 0xdf,0xef,0xf7};//3������ܿ���
			 unsigned char disbuff[4]	   ={ 0,0,0,0,};
/********************************************************/
    void Display(void)				 
	{
	 if(posit==0)
	 {P0=(discode[disbuff[posit]])&0x7f;}
	 else
	 {P0=discode[disbuff[posit]];}
	  P1=positon[posit];
	  if(++posit>=3)
	  posit=0;
	}
/********************************************************/
    void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //ʱ�任��CM
	 if((S>=700)||flag==1) //����������Χ
	 {	 
	  flag=0;
	  disbuff[0]=10;	   //��-��
	  disbuff[1]=10;	   //��-��
	  disbuff[2]=10;	   //��-��
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	 }
	}
/********************************************************/
     void zd0() interrupt 1 		 //T0װ�����������������Χ
  {
    flag=1;							 //����������Χ��ʶ
  }
/********************************************************/
   void  zd3()  interrupt 3 		 //T1ˢ����ʾ��800ms����һ��ģ��
  {
	 TH1=0xf8;
	 TL1=0x30;
	 Display();
	 timer++;
	 if(timer>=400)//
	 {
	  timer=0;
	  TX=1;			//echo�ߵ�ƽ10us����ģ��
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TX=0;
	 } 
  }
/*********************************************************/

	void  main(  void  )

  {  
    TMOD=0x11;		   //T0��ʽ1
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2ms
	TL1=0x30;
	ET0=1;             //T0�����ж�
	ET1=1;			   //T1�����ж�
	TR1=1;			   //����ʱ��
	EA=1;			   //�������ж�

	while(1)
	{
	 while(!RX);		//�ȴ����ո��ź�
	 TR0=1;			    //���ֽ��ն˸ߵ�ƽ��ʼ��ʱ
	 while(RX);			//�ȴ��ߵ�ƽ����
	 TR0=0;				//ֹͣ��ʱ
     Conut();			//�������
	}

  }