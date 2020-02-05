#include <STC89C5xRC.H>
void Delay800ms()		//@33.1776MHz
{
	unsigned char i, j, k;

	//_nop_();
	//_nop_();
	i = 101;
	j = 219;
	k = 188;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
unsigned char T0RH,T0RL;
sbit buzzer=P1^3;
void stopbuzzer();
void openbuzzer(unsigned int frequ);
void main()
{
	TMOD=0x01;
	EA=1;
	while(1)
	{
		openbuzzer(4000);Delay800ms();//4KHZ --buzzer
		openbuzzer(1000);Delay800ms();//1KHZ --buzzer.
	}
}
void openbuzzer(unsigned int frequ)
{
	unsigned int reload;
	reload=65536-(33177600/12)/(frequ*2);
	T0RH=(unsigned char)(reload>>8);
	T0RL=(unsigned char)reload;
	TH0=0xFF;
	TL0=0xFE;                        //let timer work quickly.
	ET0=1;
	TR0=1;
}
void stopbuzzer()
{
	ET0=0;TR0=0;
}
void interrupt0()  interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	buzzer=~buzzer;
}