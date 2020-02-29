#include <STC89C5xRC.H>
sbit ENLED=P1^1;
unsigned char code ledchar[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};   //nixie tube:0--F
unsigned char ledbuff[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};      //nixie tube buffer.
unsigned int T0RH,T0RL;
unsigned char byte=0;     //input number.

void configtimer0(unsigned int ms);
void ledscan();
void configUART(unsigned int baud);

void main()
{
	EA=1;
	ENLED=0;
	configtimer0(1);     //timing 1 millisecond.
	configUART(9600);    //baud=9600.
	
	while(1)//通过串口输入数字，让其在数码管上显示 
	{
		ledbuff[0]=ledchar[byte&0x0F];   //display nixie tube.   led numer transform hexadecimal.
		ledbuff[1]=ledchar[byte>>4];      
	}
}
void configtimer0(unsigned int ms)    //timing 1 millisecond.
{
	unsigned long tmp;
	tmp=33177600/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp+13;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD=0x01;
	TR0=1;
	ET0=1;
}
void configUART(unsigned int baud)   //calcualte baud  计算波特率 
{
	SCON=0x50;
	TMOD=0x20;
	TH1=256-(33177600/12/2/16)/baud;
	TL1=TH1;
	ES=1;
	ET1=0;
	TR1=1;
}
void interrupttimer0() interrupt 1  //每 1毫秒扫描一次数码管 
{
	TH0=T0RH;
	TL0=T0RL;
  ledscan();
}
void interruptUART() interrupt 4
{
	if (RI==1)//串口传送协议 
	{
		RI=0;
		byte=SBUF;
		SBUF=byte;
	}
	if(TI==1)
	{
		TI=0;
	}
}
void ledscan()  //light the nixie tube.
{
	  static unsigned char s=0;
    P0=0xFF;
		switch (s)//将通过串口发送的数据显示到数码管上 
			{
				case 0:P20=1;P21=0;P22=1;s++;P0=ledbuff[0];break;
				case 1:P20=0;P21=0;P22=1;s++;P0=ledbuff[1];break;
				case 2:P20=1;P21=1;P22=0;s++;P0=ledbuff[2];break;
				case 3:P20=0;P21=1;P22=0;s++;P0=ledbuff[3];break;
				case 4:P20=1;P21=0;P22=0;s++;P0=ledbuff[4];break;
				case 5:P20=0;P21=0;P22=0;s=0;P0=ledbuff[5];break;
				default:break;
			}
}
