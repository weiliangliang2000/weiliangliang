#include <STC89C5xRC.H>// 串口发送的数据显示到LCD当中 
unsigned char date[10];
unsigned char cotrxt=0,x=0;
void config_uart(unsigned int baud);
void lcd1602();
void Delay10ms();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str);
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
	EA=1;
	config_uart(9600);
	lcd1602();                            //init lcd1602.
	while(1)
	{
	lcd_show_str(0,0,date);                //show date in the lcd1602.
	}
}
void config_uart(unsigned int baud)    //calculate baud.
{
	SCON=0x50;
	TMOD=0x20;
	TH1=256-(33177600/12/32/baud);
	TL1=TH1;
	ET1=0;
	ES=1;
	TR1=1;
}
void interrupt_uart()  interrupt 4
{
	if(RI==1)
{
	RI=0;
	if(x<10)//将串口发送的数据储存在数组当中 
	{
		date[x]=SBUF;    //send data by UART and let lcd1602 show data.
		x++;
	}
		if(x==9)
		{
			date[x]='\0';
	  }
		SBUF=SBUF;
}
  if(TI==1)
	  TI=0;
}
