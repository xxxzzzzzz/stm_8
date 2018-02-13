#include<stdio.h>
#include<iostm8s208rb.h>

void UART1_init()
{
  UART1_CR1=0x00;//1��ֹͣλ��8������λ
  UART1_CR3=0x00;
  UART1_BRR2=0x02;//������9600bps DIV=1666d=0682h
  UART1_BRR1=0x68;
  
  asm("rim");//IAR������������Կ����жϣ�rimǰ�治��Ҫ��
  
  UART1_CR2=0x2c;//TEN=1 ������ REN=1������� ���ӽ����ж�ʹ��
  
}

void Transmit(unsigned char data)
{
  UART1_DR=data;
  while((UART1_SR&0x40)==0);//�ȴ��������
}

/*#pragma vector=UART1_R_RXNE_vector //�����жϷ������
__interrupt void UART1()   //��ѯ���ܸ�Ϊ�жϽ���
{
  unsigned char temp;
  temp=UART1_DR;
  Transmit(temp);
}*/


int fputc(int ch,FILE *f)
{
   UART1_DR=((unsigned char)ch);
   while((UART1_SR&0x40)==0);//�ȴ��������
   return ch;
}

