#include <STC89C5xRC.H>
sbit pwm=P0^0;
sbit ENLED=P1^1;
sbit A0=P2^0;           //turn on LEDS6.
sbit A1=P2^1;
sbit A2=P2^2;
unsigned char high_h,high_l,low_h,low_l;
configpwm(unsigned char hz,unsigned char dc);
void Delay1000ms()		//@33.1776MHz
{
	unsigned char i, j, k;

//	_nop_();
	//_nop_();
	i = 127;
	j = 18;
	k = 107;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	ENLED=0;
	A0=0;A1=1;A2=1;
	while(1)                // make a pwm code,it can change duty_cycle (dc) to change led's bringhtness.
	{
		configpwm(30,10);Delay1000ms();  //30hz----10% duty cycle
		EA=0;ET0=0;TR0=0;pwm=1;Delay1000ms();   //turn down pwm.
		configpwm(30,30);Delay1000ms();  //30hz----30% duty cycle
		EA=0;ET0=0;TR0=0;pwm=1;Delay1000ms();   //turn down pwm.
		configpwm(30,50);Delay1000ms();  //30hz----50% duty cycle
		EA=0;ET0=0;TR0=0;pwm=1;Delay1000ms();   //turn down pwm.
		configpwm(30,70);Delay1000ms();  //30hz----70% duty cycle
		EA=0;ET0=0;TR0=0;pwm=1;Delay1000ms();   //turn down pwm.
		configpwm(30,90);Delay1000ms();  //30hz----90% duty cycle
		EA=0;ET0=0;TR0=0;pwm=1;Delay1000ms();   //turn down pwm.
	}
}
configpwm(unsigned char hz,unsigned char dc) // claculate timer automatically.
{
	unsigned long timer;
	unsigned int high;
	unsigned int low;
	timer=(33177600/12)/hz;    
	high=65536-((timer*dc)/100)+12;
	low=65536-(timer-high)+12;
	high_h=(unsigned char)(high>>8);
	high_l=(unsigned char)high;
	low_h=(unsigned char)(low>>8);
	low_l=(unsigned char)low;
	TMOD=0x01;
	EA=1;ET0=1;
	TR0=1;
}
void interrupttime() interrupt 1
{
	if(pwm==1)
	{
		TH0=low_h;
		TL0=low_l;
		pwm=0;
	}
	else
	{
		TH0=high_h;
		TL0=high_l;
		pwm=1;
	}
		
}