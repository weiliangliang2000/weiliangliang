#include <STC89C5xRC.H>
unsigned char code i[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};//nixie tube:0--F
unsigned char order[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};// define nixie-tube's buffer cache;
unsigned int second=0;
unsigned int time=0;//define timer's time;
unsigned char s=0,in=0;
void main()
{
	ET0=1;EA=1;
	P11=0;TR0=1;TMOD=0x01;
	while(1)
	{
		if (in==1)
		{
			  in=0;
				second++;
				order[0]=i[second%10];
				order[1]=i[second/10%10];
				order[2]=i[second/100%10];
				order[3]=i[second/1000%10];
				order[4]=i[second/10000%10];
				order[5]=i[second/100000%10];
		}
	}	
}
void interrupttime() interrupt 1 //to refresh the nixie tube every 1 millisecond;
{
	TH0=0xF3;
	TL0=0x40;
	time++;
	if(time>1000)  //using timer to find every second to light nixie tube;
	{
		time=0;in=1; //return(in==1) to main;
	}
	
	P0=0xFF;
	switch (s)
			{
				case 0:P20=1;P21=0;P22=1;s++;P0=order[0];break;
				case 1:P20=0;P21=0;P22=1;s++;P0=order[1];break;
				case 2:P20=1;P21=1;P22=0;s++;P0=order[2];break;
				case 3:P20=0;P21=1;P22=0;s++;P0=order[3];break;
				case 4:P20=1;P21=0;P22=0;s++;P0=order[4];break;
				case 5:P20=0;P21=0;P22=0;s=0;P0=order[5];break;
				default:break;
			}
}