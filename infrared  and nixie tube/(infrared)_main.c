#include <STC12C5A60S2.H>
sbit ENLED=P1^1;
unsigned char code ledchar[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};//nixie tube:0--F
unsigned char ledbuff[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};// define nixie-tube's buffer cache;
unsigned char T0RH=0;
unsigned char T0RL=0;
extern bit infrared_flag;
extern unsigned char infrared_code[4];
extern void init_infrared();
void config_time0(unsigned int ms);

void main()
{
	EA=1;
	ENLED=0;
	init_infrared();
	config_time0(1);
	PT1=1;
	while(1)
	{
		if(infrared_flag==1)
		{
			infrared_flag=0;
			ledbuff[5]=ledchar[infrared_code[0]>>4];
			ledbuff[4]=ledchar[infrared_code[0]&0x0F];
		  ledbuff[1]=ledchar[infrared_code[2]>>4];
	    ledbuff[0]=ledchar[infrared_code[2]&0x0F];
		}
	}
}
void config_time0(unsigned int ms)
{
	unsigned long tmp;
	
	tmp=33177600/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+13;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD|=0x10;
	AUXR &= 0xBF;
	TH1=T0RH;
	TL1=T0RL;
	ET1=1;
	TR1=1;
}
void ledscan()
{
	static unsigned char i=0;
	P0=0xFF;
	switch (i)
			{
				case 0:P20=1;P21=0;P22=1;i++;P0=ledbuff[0];break;
				case 1:P20=0;P21=0;P22=1;i++;P0=ledbuff[1];break;
				case 2:P20=1;P21=1;P22=0;i++;P0=ledbuff[2];break;
				case 3:P20=0;P21=1;P22=0;i++;P0=ledbuff[3];break;
				case 4:P20=1;P21=0;P22=0;i++;P0=ledbuff[4];break;
				case 5:P20=0;P21=0;P22=0;i=0;P0=ledbuff[5];break;
				default:break;
			}
}
void interrupttime0() interrupt 3
			{
				TH1=T0RH;
				TL1=T0RL;
				ledscan();
			}