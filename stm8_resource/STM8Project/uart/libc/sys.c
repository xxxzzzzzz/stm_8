#include<iostm8s208rb.h>

/************延时ms函数************/
void Delay_ms(unsigned int ms)
{
	unsigned int x,i;
	for(x=ms;x>0;x--)
	{
		for(i=1550;i>0;i--); 
	}
}

/************延时3us函数************/
void Delay_us(unsigned int us)
{
	
	while(us--);
	
}


void CLK_start()
{
  CLK_SWCR = 0x02;//使能时钟切换
  CLK_SWR = 0xb4;//时钟源为HSE 
}