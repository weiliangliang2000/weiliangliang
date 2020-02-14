#include <STC89C5xRC.H>
void i2c_start();
void i2c_stop();
bit i2c_write(unsigned char dat);
unsigned char i2c_read_not_ack();
unsigned char i2c_read_ack();
void init_lcd1602();
void Delay10ms();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str);
void write_byte(unsigned char addr,unsigned char dat);
unsigned char read_byte(unsigned char addr);
void i2c_delay();
void Delay10ms()		//@33.1776MHz
{
	unsigned char i, j, k;

	//_nop_();
	//_nop_();
	i = 2;
	j = 67;
	k = 183;
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
	unsigned int dat=100;
	unsigned char str[10];
	
	init_lcd1602();
	dat=read_byte(0x02);
	str[0]=(dat/100)+'0';
	str[1]=(dat/10%10)+'0';
	str[2]=(dat%10)+'0';
	str[3]='\0';
	lcd_show_str(0,0,str);
	dat++;
	write_byte(0x02,dat);
	while(1);
}
unsigned char read_byte(unsigned char addr)
{
	unsigned char dat;
	
	i2c_start();
	i2c_write(0x51<<1);
	i2c_write(addr);
	i2c_start();
	i2c_write((0x51<<1)|0x01);
	dat=i2c_read_not_ack();
	i2c_stop();
	
	return dat;
}
void write_byte(unsigned char addr,unsigned char dat)
{
	i2c_start();
	i2c_write(0x51<<1);
	i2c_write(addr);
	i2c_write(dat);
	i2c_stop();
}