#include <iostm8s208rb.h>

const unsigned char  table0[]={0x3f,0x06,0x5b,0x4f,0x66,    
						                   0x6d,0x7d,0x07,0x7f,0x6f};   // ��������޵�
const unsigned char  table1[]= {0xbf,0x86,0xdb,0xcf,0xe6,   
																0xed,0xfd,0x87,0xff,0xef};  // ��������е�
 												
unsigned char NUM;                //������ʾ����






void Delay_Ms(unsigned int ms);
void Seg_Init(void);
void EXTI_init(void);
void Display(unsigned int num);
int main( void )
{
  CLK_SWCR |=0x02;//��ʱ���л���HSE��
  CLK_SWR= 0xB4; //ʱ��ԴΪHSE 0xB4
  EXTI_init();//�жϳ�ʼ��
  Seg_Init();
  
  
  while(1)
  {
    Display(NUM);
  }
  
}

//////�жϳ�ʼ������///////
void EXTI_init(void)
{
  PD_DDR=0x00; //PD2Ϊ����ģʽ
  PD_CR1=0x04; //0000 0100 ����������
  PD_CR2=0x04; //0000 0100 ʹ��PD2���ⲿ�ж�
  
  EXTI_CR1=0x80; //1000 0000 PD���½��ش���
  
  asm("rim"); //��ȫ���ж� IAR�Ļ������asmǰ�治��Ҫб�ܣ�
  
}


/************����ܳ�ʼ������************/
void Seg_Init(void)
{
  PB_DDR  = 0xFF;    // ��PB�����ó��������
  PB_CR1  = 0xFF;              
  PB_CR2  = 0x00;

	PF_DDR |= 0xF0;    // ��PF�ڸ���λ���ó��������
  PF_CR1 |= 0xF0;              
  PF_CR2 &= 0x00;
	
	PE_DDR |= 0x01;    // ��PE0���ó�������� SEGEN
  PE_CR1 |= 0x01;              
  PE_CR2 &= 0x00;
	
  PE_ODR &= 0xFE;    // PE0=0��ʹ�������
}


/************��ʾ����************/
void Display(unsigned int num)
{
  unsigned char GeWei,ShiWei,BaiWei,QianWei; 

	GeWei = num%10;       
	ShiWei = num%100/10;
	BaiWei = num%1000/100; 
	QianWei = num/1000;      

	PB_ODR = table0[GeWei];
	PF_ODR = 0xE0;
	Delay_Ms(3);
	PB_ODR = 0;
	PF_ODR = 0xF0;
	Delay_Ms(1);

	PB_ODR = table0[ShiWei];
	PF_ODR = 0xD0;
	Delay_Ms(3);
	PB_ODR = 0;
	PF_ODR = 0xF0;
	Delay_Ms(1);

	PB_ODR = table0[BaiWei];
	PF_ODR = 0xB0;
	Delay_Ms(3);
	PB_ODR = 0;
	PF_ODR = 0xF0;
	Delay_Ms(1);

//	PB_ODR = table0[QianWei];
//	PF_ODR = 0x70;
//	Delay_Ms(3);
//	PB_ODR = 0;
//	PF_ODR = 0xF0;
//	Delay_Ms(1);
}
/************����************/

/************��ʱMS����************/
void Delay_Ms(unsigned int ms)
{
	unsigned int x,y;
	for(x=ms;x>0;x--)
	{
		for(y=300;y>0;y--)
		{
		}
	}
}
/************��ʱuS����************/
void Delay_Us(unsigned char t)
{
	unsigned char m=t;
	while(m--);
}

/*�жϷ�����*/
#pragma vector=EXTI3_vector
__interrupt void EXTI3(void)
{
 NUM++;
 
}
