#include <iostm8s208rb.h>
int main( void )
{
  PC_DDR = 0x0E;   //PC3 ~ PC1Ϊ���0000 1110
  PC_CR1 = 0x0E;   //0000 1110
  PC_CR2 = 0x00;
  
  PI_DDR = 0x01;
  PI_CR1 = 0x01;
  PI_CR2 = 0x00;
  
  PI_ODR = 0x00; //PIO����͵�ƽ
  PC_ODR = 0x02; //PC1����ߵ�ƽ
  while(1);
  
}
