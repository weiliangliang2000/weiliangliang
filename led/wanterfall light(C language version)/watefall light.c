#include <STC89C5xRC.H>
void main()
{
	unsigned int i=0;
	unsigned char z=0;
	P20=0;
	P21=1;
	P22=1;//we set LEDS6 at a low potential.
	P11=0;//enabled
	while(1)
	{
		P0=~(0X01<<z);//�����������ε���led 
		for (i=0;i<60000;i++);//led������ʱ�䡣 
		{
		  z++;
		}
		if(z>=8)
		{
			z=0;
	  }
  }
}
