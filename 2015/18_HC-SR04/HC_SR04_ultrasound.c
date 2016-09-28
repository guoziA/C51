
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
			 unsigned char const discode[] ={ 0x50,0x5f,0x38,0x19,0x17,0x91,0x90,0x5d,0x10,0x11,0xef/*-*/};//数字0~9
			 unsigned char const positon[3]={ 0xdf,0xef,0xf7};//3个数码管开关
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
	
	 S=(time*1.7)/100;     //时间换成CM
	 if((S>=700)||flag==1) //超出测量范围
	 {	 
	  flag=0;
	  disbuff[0]=10;	   //“-”
	  disbuff[1]=10;	   //“-”
	  disbuff[2]=10;	   //“-”
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	 }
	}
/********************************************************/
     void zd0() interrupt 1 		 //T0装满溢出，超过测量范围
  {
    flag=1;							 //超出测量范围标识
  }
/********************************************************/
   void  zd3()  interrupt 3 		 //T1刷新显示，800ms启动一次模块
  {
	 TH1=0xf8;
	 TL1=0x30;
	 Display();
	 timer++;
	 if(timer>=400)//
	 {
	  timer=0;
	  TX=1;			//echo高电平10us启动模块
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
    TMOD=0x11;		   //T0方式1
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2ms
	TL1=0x30;
	ET0=1;             //T0允许中断
	ET1=1;			   //T1允许中断
	TR1=1;			   //开定时器
	EA=1;			   //总允许中断

	while(1)
	{
	 while(!RX);		//等待接收高信号
	 TR0=1;			    //出现接收端高电平开始计时
	 while(RX);			//等待高电平结束
	 TR0=0;				//停止计时
     Conut();			//计算距离
	}

  }