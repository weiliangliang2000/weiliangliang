#include <STC89C5xRC.H>
#define lcd_DB P0
sbit lcd_RS=P2^0;
sbit lcd_RW=P2^1;
sbit lcd_E=P1^2;
void lcd1602();
void Delay10ms();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str);

void lcd_write_ready()
{
	unsigned char sta;
	lcd_DB=0xFF;
	Delay10ms();
	lcd_RS=0;
	lcd_RW=1;
	do{
	    lcd_E=1;
			sta=lcd_DB;
			lcd_E=0;
		}while(sta&0x80);
}
void lcd_write_command(unsigned char command)
{
	lcd_write_ready();
	lcd_RS=0;
	lcd_RW=0;
	lcd_DB=command;
	Delay10ms();
	lcd_E=1;
	lcd_E=0;
}
void lcd_show_date(unsigned char date)
{
	lcd_write_ready();
	lcd_RS=1;
	lcd_RW=0;
	lcd_DB=date;
	Delay10ms();
	lcd_E=1;
	lcd_E=0;
}
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str)
{
	unsigned char address;
	if(y==0)
	{
		address=0x00+x;
	}
	else
	{
		address=0x40+x;
	}
	lcd_write_command(address|0x80);
	while(*str!='\0')
	{
		lcd_show_date(*str++);    //send byte to the lcd1602
	}
}
void lcd1602()    //init lcd1602.
{
	lcd_write_command(0x38);
	lcd_write_command(0x0C);
	lcd_write_command(0x06);
	lcd_write_command(0x01);
}