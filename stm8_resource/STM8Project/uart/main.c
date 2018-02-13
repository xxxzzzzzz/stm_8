#include<iostm8s208rb.h>
#include<stdio.h>
#include"sys.h"
#include"uart.h"







int main( void )
{
  unsigned int i;
  unsigned char c;
  unsigned short s;
  unsigned long l;
  int p;
  
  CLK_start();
  UART1_init();
  
  
  while(1)
  {
   p=sizeof(i);
   printf("int = %d\r\n",p);
   p=sizeof(c);
   printf("char = %d\r\n",p);
   p=sizeof(s);
   printf("short = %d\r\n",p);
   p=sizeof(l);
   printf("long = %d\r\n",p);
   printf("STM8µ¥Æ¬»úÊä³ö\r\n");
   Delay_ms(1000);
   
   
  }
   
}
