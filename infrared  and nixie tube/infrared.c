#include <STC12C5A60S2.H>
sbit IRD=P3^2;
bit infrared_flag=0;
unsigned char infrared_code[4];

void init_infrared()
{
	IRD=1;
	TMOD|=0x01;
	//AUXR &= 0x7F;
	TR0=0;
	ET0=0;
	IT0=1;
	EX0=1;
}
unsigned int get_high_time()
{
	TH0=0;
	TL0=0;
	TR0=1;
	while(IRD)
	{
		if(TH0>=0xC2)
		{
			break;
		}
	}
	TR0=0;
	return(TH0*256+TL0);
}
unsigned int get_low_time()
{
	TH0=0;
	TL0=0;
	TR0=1;
	while(!IRD)
	{
		if(TH0>=0xC2)
		{
			break;
		}
	}
	TR0=0;
	return(TH0*256+TL0);
}
void exint() interrupt 0
{
	unsigned char i,j;
	unsigned char byte;
	unsigned int time;
	
	time=get_low_time();
	if((time<23500)||(time>26265))      //8.5/(12/33177600)
	{
		IE0=0;
		return;
	}
	time=get_high_time();
	if((time<11059)||(time>13824))
	{
		IE0=0;
		return;
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			time=get_low_time();
			if((time<940)||(time>2156))
			{
				IE0=0;
				return;
			}
			time=get_high_time();
			if((time>940)&&(time<2156))
			{
				byte>>=1;
			}
			else if ((time>4036)&&(time<5253))
			{
				byte>>=1;
				byte|=0x80;
			}
			else
			{
				IE0=0;
				return;
			}
		}
		infrared_code[i]=byte;
	}
	infrared_flag=1;
	IE0=0;
}