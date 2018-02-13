#include <iostm8s208rb.h>

const unsigned char  table0[]={0x3f,0x06,0x5b,0x4f,0x66,    
						                   0x6d,0x7d,0x07,0x7f,0x6f};   // 共阴码表无点
const unsigned char  table1[]= {0xbf,0x86,0xdb,0xcf,0xe6,   
																0xed,0xfd,0x87,0xff,0xef};  // 共阴码表有点
 												
unsigned char NUM;                //定义显示变量






void Delay_Ms(unsigned int ms);
void Seg_Init(void);
void EXTI_init(void);
void Display(unsigned int num);
int main( void )
{
  CLK_SWCR |=0x02;//将时钟切换到HSE上
  CLK_SWR= 0xB4; //时钟源为HSE 0xB4
  EXTI_init();//中断初始化
  Seg_Init();
  
  
  while(1)
  {
    Display(NUM);
  }
  
}

//////中断初始化函数///////
void EXTI_init(void)
{
  PD_DDR=0x00; //PD2为输入模式
  PD_CR1=0x04; //0000 0100 带上拉输入
  PD_CR2=0x04; //0000 0100 使能PD2口外部中断
  
  EXTI_CR1=0x80; //1000 0000 PD口下降沿触发
  
  asm("rim"); //开全局中断 IAR的汇编语言asm前面不需要斜杠，
  
}


/************数码管初始化函数************/
void Seg_Init(void)
{
  PB_DDR  = 0xFF;    // 将PB口设置成推挽输出
  PB_CR1  = 0xFF;              
  PB_CR2  = 0x00;

	PF_DDR |= 0xF0;    // 将PF口高四位设置成推挽输出
  PF_CR1 |= 0xF0;              
  PF_CR2 &= 0x00;
	
	PE_DDR |= 0x01;    // 将PE0设置成推挽输出 SEGEN
  PE_CR1 |= 0x01;              
  PE_CR2 &= 0x00;
	
  PE_ODR &= 0xFE;    // PE0=0，使能数码管
}


/************显示函数************/
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
/************结束************/

/************延时MS函数************/
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
/************延时uS函数************/
void Delay_Us(unsigned char t)
{
	unsigned char m=t;
	while(m--);
}

/*中断服务函数*/
#pragma vector=EXTI3_vector
__interrupt void EXTI3(void)
{
 NUM++;
 
}
