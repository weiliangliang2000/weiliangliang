#include <STC12C5A60S2.H>
bit flag_1s=0;
unsigned char high=0;
unsigned char low=0;
void config_time0(unsigned int ms);
unsigned char init_string(unsigned char *str,int dat);

void init_lcd1602();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str);
void Delay10ms();		//@33.1776MHz

void delay(unsigned char t);
bit get_ds18b20_ack();
void write_ds18b20(unsigned char dat);
unsigned char read_ds18b20();
bit start_ds18b20();
bit get_ds18b20_temp(int *temp);

void main()
{
	bit res;
	int temp;
	int intt,dect;
	unsigned char len;
	unsigned char str[12];
	
	EA=1;
	config_time0(10);
	start_ds18b20();
	init_lcd1602();
	
	while(1)
	{
		if(flag_1s)
		{
			flag_1s=0;
			res=get_ds18b20_temp(&temp);
			if(res)
			{
				intt=temp>>4;
				dect=temp&0x0f;
				len=init_string(str,intt);
				str[len++]='.';
				dect=(dect*10)/16;
				str[len++]=dect+'0';
				while(len<6)
				{
					str[len++]=' ';
				}
				str[len]='\0';
				lcd_show_str(0,0,str);
			}
			else
			{
				lcd_show_str(0,0,"error!");
			}
			start_ds18b20();
		}
		
	}
}
unsigned char init_string(unsigned char *str,int dat)
{
	signed char i=0;
	unsigned char len=0;
	unsigned char buf[6];
	
	if(dat<0)
	{
		dat=-dat;
		*str++='-';
		len++;
	}
	do{
		buf[i++]=dat %10;
		dat/=10;
	}while(dat>0);
	len+=i;
	while(i-->0)
	{
		*str++=buf[i]+'0';
	}
	*str='\0';
	return len;
}
void config_time0(unsigned int ms)
{
	unsigned long tmp;
	
	tmp=33177600/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+13;
	high=(unsigned char)(tmp>>8);
	low=(unsigned char)tmp;
	TMOD|=0x01;
	AUXR &= 0xBF;
	TH0=high;
	TL0=low;
	ET0=1;
	TR0=1;
}
void interrupttime0() interrupt 1
{
	static unsigned char time=0;
	TH0=high;
	TL0=low;
	time++;
	if(time>=100)
	{
		time=0;
		flag_1s=1;
	}	
}