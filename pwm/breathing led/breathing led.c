#include <STC89C5xRC.H>    //this is a breathing led.i think it is a successful production,but its effect is very quickly.
sbit pwm=P1^3;
sbit ENLED=P1^1;
sbit A0=P2^0;             //turn on LEDS6.
sbit A1=P2^1;
sbit A2=P2^2;
unsigned char high_h,high_l,low_h,low_l;
configpwm(unsigned char hz,unsigned char dc);
unsigned long timer;
unsigned char T1RH,T1RL;

configtimer(unsigned int ms);
adjust_duty_cycle(unsigned char dc);

configtimer(unsigned int ms)      //timer_2,timing 1000ms.
{
	unsigned long tmp;
		
	tmp=33177600/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp+12;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD=0x10;
	ET1=1;                     //turn on interruption_2
	TR1=1;                    //turn on timer_2
}

void main()
{
	ENLED=0;
	A0=0;A1=1;A2=1;
	
	configpwm(300,10);           //3000hz------10% duty cycle.
	configtimer(500);
	while(1) ;
}		                                         
configpwm(unsigned char hz,unsigned char dc)    // claculate timer automatically.
{
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
	EA=1;ET0=1;                            //turn on interruption
	TR0=1;                                //turn on timer_1
}

adjust_duty_cycle(unsigned char dc)
{   
	unsigned int high;
  unsigned int low;
	
	high=65536-((timer*dc)/100)+12;
	low=65536-(timer-high)+12;
	high_h=(unsigned char)(high>>8);
	high_l=(unsigned char)high;
	low_h=(unsigned char)(low>>8);
	low_l=(unsigned char)low;
}

void interrupttime() interrupt 1    //use timer_1 to contral to light the led.
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

void interrupttime1() interrupt 3
{
	    static unsigned char index=0;
	    unsigned char breathing_led[19]={5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};   // make a pwm code,it can change duty_cycle (dc) to change led's bringhtness.
		  static bit dir=0;
		  TH1=T1RH;
			TL1=T1RL;
		  adjust_duty_cycle(breathing_led[index]); //change pwm's duty cycle.
			if(dir==0)
			{
				index++;               //increase duty cycle gradually.
				if(index>=18)
				{
					dir=1;
				}
			}
			else
			{
				index--;              //decrease duty cycle gradually.
				if(index==0)
				{
					dir=0;
				}
			}
			
}