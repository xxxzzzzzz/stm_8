
#include <iostm8s208rb.h>

void delay_ms(unsigned int ms);//��ʱ����

int main( void )
{ 
  unsigned char temp=0;
  PC_DDR = 0x0E;   //PC3 ~ PC1Ϊ���0000 1110
  PC_CR1 = 0x0E;   //0000 1110
  PC_CR2 = 0x00;
  
  PI_DDR = 0x01;
  PI_CR1 = 0x01;
  PI_CR2 = 0x00;
  
  PI_ODR = 0x00; //PIO����͵�ƽ
  PC_ODR = 0x02; //PC1����ߵ�ƽ
  
                 
  
  
  while(1)
  {
    temp++;
    if(temp>5)
    {
      CLK_SWCR |=0x02;//��ʱ���л���HSE��
      CLK_SWR= 0xB4; //ʱ��ԴΪHSE 0xB4
      
      
    }
    PC_ODR = 0x02; //PC1����ߵ�ƽ
    delay_ms(1000);
    PC_ODR = 0x04; //PC2����ߵ�ƽ
    delay_ms(1000);
    PC_ODR = 0x08; //PC3����ߵ�ƽ
    delay_ms(1000);
  }
  
}


void delay_ms(unsigned int ms)
{
  unsigned int x,y;
  for(x=ms;x>0;x--)
  {
    for(y=300;y>0;y--)
    {
      
    }
  }
}
