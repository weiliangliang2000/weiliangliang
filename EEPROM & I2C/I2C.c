#include <STC89C5xRC.H>
#include <intrins.h>
void i2c_delay() 
{
	unsigned char i;    //delay 5 us.

	i = 39;
	while (--i);
}

sbit scl=P3^6;
sbit sda=P3^7;

void i2c_start();
void i2c_stop();
bit i2c_write(unsigned char dat);
unsigned char i2c_read_not_ack();
unsigned char i2c_read_ack();

void i2c_start()
{
	sda=1;
	scl=1;
  i2c_delay();
	sda=0;
	i2c_delay();
	scl=0;
}
void i2c_stop()
{
	scl=0;
	sda=0;
	i2c_delay();
	scl=1;
	i2c_delay();
	sda=1;
	i2c_delay();
}
bit i2c_write(unsigned char dat)
{
	bit ack;
	unsigned char mask;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		sda=(mask&dat);
		i2c_delay();
		scl=1;
		i2c_delay();
		scl=0;
	}
	sda=1;
	i2c_delay();
	scl=1;
	ack=sda;
	i2c_delay();
	scl=0;
	
	return (~ack);
}
unsigned char i2c_read_not_ack()
{
	unsigned char mask;
	unsigned char dat;
	sda=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		i2c_delay();
		scl=1;
		if(sda==0)
			dat &= ~mask;
		else
			dat |= mask;
		i2c_delay();
		scl=0;
	}
	sda=1;
	i2c_delay();
	scl=1;
	i2c_delay();
	scl=0;
	
	return dat;
}
unsigned char i2c_read_ack()
{
	unsigned char mask;
	unsigned char dat;
	sda=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		i2c_delay();
		scl=1;
		if(sda==0)
			dat &= ~mask;
		else
			dat |=mask;
		i2c_delay();
		scl=0;
	}
	sda=0;
	i2c_delay();
	scl=1;
	i2c_delay();
	scl=0;
	
	return dat;
}