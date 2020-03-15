#include <STC12C5A60S2.H>
#include <intrins.h>
void init_lcd1602();
void lcd_write_ready();
void lcd_write_command(unsigned char command);
void lcd_show_str(unsigned char x,unsigned char y,unsigned char *str);

unsigned char str[10];

typedef unsigned char BYTE;
typedef unsigned int WORD;

//sfr ADC_CONTR=0XBC;
//sfr ADC_RES=0XBD;
//sfr ADC_LOW2=0XBE;
sfr PLASF=0X9D;

#define ADC_POWER 0X80
#define ADC_FLAG 0X10
#define ADC_START 0X08
#define ADC_SPEEDLL 0X00

void SendData(BYTE dat);
void Delay(WORD n);
void InitADC();

void main()
{
	P10=0;
	init_lcd1602();
	InitADC();
	IE=0XA0;
	lcd_show_str(0,1,str);
	while(1);
}

void adc_isr() interrupt 5 using 1
{
	unsigned char dat;
	ADC_CONTR&=!ADC_FLAG;
	dat=ADC_RES;
	dat=(dat*50)/255;
	str[0]='V';
	str[1]='1';
	str[2]=':';
	str[3]=(dat/10)+'0';
	str[4]='.';
	str[5]=(dat/10%10)+'0';
	str[6]=(dat%10)+'0';
	str[7]='V';
	str[8]='\0';
	
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ADC_START;
}
void InitADC()
{
	P1ASF=0X01;
	ADC_RES=0;
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ADC_START;
	Delay(2);
}
void Delay(WORD n)
{
	WORD x;
	while(n--)
	{
		x=5000;
		while(x--);
	}
}