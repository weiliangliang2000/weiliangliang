#include <STC12C5A60S2.H>
#include <intrins.h>
sbit ds18b20=P3^5;

void delay(unsigned char t)		//@33.1776MHz  delay 1 us.
{
	unsigned char i;
 do
	 {
	    _nop_();
	    _nop_();
	    _nop_();
    	i = 5;
	    while (--i);
   }while(--t);
}
bit get_ds18b20_ack()
{
	bit ack;
	EA=0;
	ds18b20=0;
	delay(500);
	ds18b20=1;
	delay(60);
	ack=ds18b20;
	while(!ds18b20);
	EA=1;
	
	return ack;
}
void write_ds18b20(unsigned char dat)
{
	unsigned char mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)
	{
		ds18b20=0;
		delay(2);
		if((mask&dat)==0)
			ds18b20=0;
		else
			ds18b20=1;
		delay(60);
		ds18b20=1;
	}
	EA=1;
}
unsigned char read_ds18b20()
{
	unsigned char dat,mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)
	{
		ds18b20=0;
		delay(2);
		ds18b20=1;
		delay(2);
		if(!ds18b20)
			dat&=~mask;
		else
			dat|=mask;
		delay(60);
	}
	EA=1;
	return dat;
}
bit start_ds18b20()
{
	bit ack;
	ack=get_ds18b20_ack();
	if(ack==0)
	{
		write_ds18b20(0xcc);
		write_ds18b20(0x44);
	}
	return ~ack;
}
bit get_ds18b20_temp(int *temp)
{
	bit ack;
	unsigned char lsb,msb;
	ack=get_ds18b20_ack();
	if(ack==0)
	{
		write_ds18b20(0xcc);
		write_ds18b20(0xbe);
		lsb=read_ds18b20();
		msb=read_ds18b20();
		*temp=((int)msb<<8)+lsb;
	}
	return ~ack;
}