#include<iostm8s208rb.h>

/************��ʱms����************/
void Delay_ms(unsigned int ms)
{
	unsigned int x,i;
	for(x=ms;x>0;x--)
	{
		for(i=1550;i>0;i--); 
	}
}

/************��ʱ3us����************/
void Delay_us(unsigned int us)
{
	
	while(us--);
	
}


void CLK_start()
{
  CLK_SWCR = 0x02;//ʹ��ʱ���л�
  CLK_SWR = 0xb4;//ʱ��ԴΪHSE 
}