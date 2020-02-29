#include <STC89C5xRC.H>//在LCD上实现数据滚动显示 
#define lcd_DB P0
sbit lcd_RS=P2^0;
sbit lcd_RW=P2^1;
sbit lcd_E=P1^2;
unsigned char flag500ms=0,time=0;
void lcd1602();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str,unsigned char l);
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
	unsigned char i,index;
	unsigned char code str1[]="happy new year";//要在屏幕上显示的数据 
	unsigned char code str2[]="beautiful.....";
	unsigned char pdata lcd_move_buffer1[16+sizeof(str1)+16];
	unsigned char pdata lcd_move_buffer2[16+sizeof(str2)+16];
	
	lcd_E=0;
	lcd1602();   // init lcd1602.
	
	AUXR &= 0x7F;		
	TMOD &= 0xF0;		
	TMOD |= 0x01;		
	TL0 = 0x00;		//timing 10 ms.
	TH0 = 0x94;		
	TF0 = 0;		
	TR0 = 1;
  EA=1;
	ET0=1;
	//缓冲区开头一段填充为空格 
	for(i=0;i<16;i++)
	{
		lcd_move_buffer1[i]=' ';
		lcd_move_buffer2[i]=' ';
	}
	//待显示字符串复制到中间位置 
	for(i=0;i<sizeof(str1)-1;i++)
	{
		lcd_move_buffer1[16+i]=str1[i];
		lcd_move_buffer2[16+i]=str2[i];
	}
	//缓冲区结尾部分也填充为空格 
	for(i=16+sizeof(str1)-1;i<sizeof(lcd_move_buffer1);i++)
	{
		lcd_move_buffer1[i]=' ';
		lcd_move_buffer2[i]=' ';
	}
	while(1)
	{
		if(flag500ms)
		{
			flag500ms=0;
			//缓冲区抽取需显示的一段字符显示到液晶上 
			lcd_show_str(0,0,lcd_move_buffer1+index,16);
	        lcd_show_str(0,1,lcd_move_buffer2+index,16);
	        //移动索引递增，实现左移 
			index++;
			if(index>16+sizeof(str1))
			{
				index=0;//起始位置达到字符串尾部后返回从头开始 
			}
		} 
	}
}
void interrupt_time_10ms() interrupt 1
{
	TL0 = 0x00;		
	TH0 = 0x94;	
	time++;
	if(time>50)//500毫秒移动一次 
	{
		flag500ms=1;
	}
}
void lcd_show_date(unsigned char date)//将数据显示出来 
{
	lcd_write_ready();
	lcd_RS=1;
	lcd_RW=0;
	lcd_DB=date;
	Delay10ms();
	lcd_E=1;
	lcd_E=0;
}
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str,unsigned char l)
{
	unsigned char address;//利用坐标将数据显示到屏幕想要的位置上 
	if(y==0)
	{
		address=0x00+x;
	}
	else
	{
		address=0x40+x;
	}
	lcd_write_command(address|0x80);
	while(l--)
	{
		lcd_show_date(*str++);//将数据以指针形式传送到显示函数当中 
	}
	
}
void lcd_write_ready()//判断LCD是否忙碌 
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
void lcd_write_command(unsigned char command)//写入命令---LCD 
{
	lcd_write_ready();
	lcd_RS=0;
	lcd_RW=0;
	lcd_DB=command;
	Delay10ms();
	lcd_E=1;
	lcd_E=0;
}
void lcd1602()    //init lcd1602.
{
	lcd_write_command(0x38);
	lcd_write_command(0x0C);
	lcd_write_command(0x06);
	lcd_write_command(0x01);
}
