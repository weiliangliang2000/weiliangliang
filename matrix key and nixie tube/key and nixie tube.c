#include <STC89C5xRC.H>
unsigned char code i[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};//nixie tube:0--F		
bit buff=1;
	void main()
{
	bit b=1;
	unsigned char count=0;
	
	TMOD=0x01;
	TR0=1;
	TH0=0xF5;
	TL0=0x32;
	EA=1; ET0=1;//turn on the interruption
	P11=0;
	P20=1;
	P21=0;
	P22=1;//turn on the fifth nixie tube.
	P0=i[count];
	
	P27=0;//P27=key4
	P23=1;
	
	while(1)
	{
		if(buff!=b)//check out the status of the key4.
		{
			b=buff;
			if(b==0)
			{
				count++;
				if(count>9)
				{
					count=0;
				}
				P0=i[count];
			}
		}
	}
}
void interrupttime() interrupt 1
{
	static unsigned char compare=0xFF;
	TH0=0xF5;//refresh interruption every 1 millisecond
	TL0=0x32;
	compare=(compare<<1)|P23;
	if(compare==0xFF)//eliminate error in order to improve the key4.
	{
		buff=1;
	}
	else if(compare==0x00)
	{
		buff=0;
	}
	else
   {}
}