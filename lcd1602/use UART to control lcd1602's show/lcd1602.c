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

void lcd_write_ready()  //判断LCD是否准备好接受数据 
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
void lcd_write_command(unsigned char command)//将命令写入到LCD当中 
{
	lcd_write_ready();
	lcd_RS=0;
	lcd_RW=0;
	lcd_DB=command;
	Delay10ms();
	lcd_E=1;
	lcd_E=0;
}
void lcd_show_date(unsigned char date)//将数据显示到LCD上 
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
	unsigned char address;//利用坐标轴的方法，数据可以显示到LCD屏幕想要的位置上 
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
		lcd_show_date(*str++);    //send byte to the lcd1602   将数据以指针的形式传送到数据显示函数里 
	}
}
void lcd1602()    //init lcd1602.初始化LCD 
{
	lcd_write_command(0x38);//16*2显示，5*7点阵，8位数据接口 
	lcd_write_command(0x0C);//显示器开，光标关闭 
	lcd_write_command(0x06);//文字不动，地址自动＋1 
	lcd_write_command(0x01);//清屏 
}
