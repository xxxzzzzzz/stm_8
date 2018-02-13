#include<stdio.h>
#include<iostm8s208rb.h>

void UART1_init()
{
  UART1_CR1=0x00;//1个停止位，8个数据位
  UART1_CR3=0x00;
  UART1_BRR2=0x02;//波特率9600bps DIV=1666d=0682h
  UART1_BRR1=0x68;
  
  asm("rim");//IAR编译器汇编语言开总中断，rim前面不需要杠
  
  UART1_CR2=0x2c;//TEN=1 允许发送 REN=1允许接收 增加接受中断使能
  
}

void Transmit(unsigned char data)
{
  UART1_DR=data;
  while((UART1_SR&0x40)==0);//等待发送完成
}

/*#pragma vector=UART1_R_RXNE_vector //串口中断服务程序
__interrupt void UART1()   //轮询接受改为中断接受
{
  unsigned char temp;
  temp=UART1_DR;
  Transmit(temp);
}*/


int fputc(int ch,FILE *f)
{
   UART1_DR=((unsigned char)ch);
   while((UART1_SR&0x40)==0);//等待发送完成
   return ch;
}

