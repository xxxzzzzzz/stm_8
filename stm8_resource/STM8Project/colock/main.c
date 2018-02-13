
#include <iostm8s208rb.h>

void delay_ms(unsigned int ms);//延时函数

int main( void )
{ 
  unsigned char temp=0;
  PC_DDR = 0x0E;   //PC3 ~ PC1为输出0000 1110
  PC_CR1 = 0x0E;   //0000 1110
  PC_CR2 = 0x00;
  
  PI_DDR = 0x01;
  PI_CR1 = 0x01;
  PI_CR2 = 0x00;
  
  PI_ODR = 0x00; //PIO输出低电平
  PC_ODR = 0x02; //PC1输出高电平
  
                 
  
  
  while(1)
  {
    temp++;
    if(temp>5)
    {
      CLK_SWCR |=0x02;//将时钟切换到HSE上
      CLK_SWR= 0xB4; //时钟源为HSE 0xB4
      
      
    }
    PC_ODR = 0x02; //PC1输出高电平
    delay_ms(1000);
    PC_ODR = 0x04; //PC2输出高电平
    delay_ms(1000);
    PC_ODR = 0x08; //PC3输出高电平
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
