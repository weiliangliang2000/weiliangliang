#include <STC89C5xRC.H>
void main()
{
	unsigned char time=0,i=0;
	P20=0;
	P21=1;
	P22=1;//we set LEDS6 at a low potential.
	P11=0;//enabled
	
	TMOD=0x01;
	TR0=1;//turn on the timer.
	TL0=0x27;//calculate the initial value of 20 milliseconds.
	TH0=0xFF;
	
	while(1)
	{
		if(TF0==1)//return to position after each loop.
		{
			TF0=0;
			TH0=0x27;
			TL0=0xFF;
			time++;
			if(time>=50)//light the led every second.
			{
				time=0;
			P0=~(0x01<<i);//light every led.
				i++;
				if(i>=8)
				{
					i=0;
				}
			}
		}
	}
}