#include<iostm8s208rb.h>
#include<uart.h>
#include "stm8s.h"//引入STM8的头文件
#include "stm8s_can.h"
#include "stm8s_clk.h"
#include "stm8s_it.h"
struct CAN_RX_buf
{
    unsigned char ID[4];
    unsigned char data[8];
};

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


void CAN_FilterInit_socket(unsigned long ID)//CAN固件库接口封装
{
  unsigned char FxR1,FxR2,FxR3,FxR4;
  FxR1 = (unsigned char)(((ID>>24)&0x0f)<<3);
  FxR1 = (unsigned char)((((ID>>16)>>5)&0x07)|FxR1);
  FxR2 = (unsigned char)(((ID>>16)<<1)&0xe7);
  FxR2 = (unsigned char)((((ID>>8)>>7)&0x01)|FxR2);
  FxR3 = (unsigned char)((ID>>8)<<1);
  FxR3 = (unsigned char)(((ID>>7)&(0x01))|FxR3);
  FxR4 = (unsigned char)((ID<<1)&0xfe);
  
  CAN_FilterInit(CAN_FilterNumber_0,ENABLE,CAN_FilterMode_IdMask,CAN_FilterScale_32Bit,FxR1,FxR2,FxR3,FxR4,0x0f,0,0xff,0xff);
}

/*********************************************************************************
*   函 数 名: CAN_Configuration
*   功能说明: CAN初始化
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void CAN_Configuration(void)
{
    //初始化CAN波特率为1M  
  /*CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_11TimeQuantum,
             CAN_BitSeg2_4TimeQuantum,1);*/
  //初始化CAN波特率为800K  
 /* CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_5TimeQuantum,
             CAN_BitSeg2_4TimeQuantum,2);*/
  
  //初始化CAN波特率为500K  
  /*CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,4);*/
  
  //初始化CAN波特率为250K  
  CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,8);
  
  //初始化CAN波特率为125K  
 /* CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_8TimeQuantum,
             CAN_BitSeg2_7TimeQuantum,8);*/
  
    //配置CAN第0组过滤器为屏蔽模式。但是屏蔽位都设置为0，所以相当于没启动屏蔽功能。
  //CAN_FilterInit(CAN_FilterNumber_0,ENABLE,CAN_FilterMode_IdMask,CAN_FilterScale_32Bit,0,0,0x7f,0xff,0,0,0xff,0xff);
    CAN_FilterInit_socket(0x3efe);
    
    //使能CAN接收中断功能
    //CAN_ITConfig(CAN_IT_FMP,ENABLE);
    
   
}



/*********************************************************************************
*   函 数 名: CAN_RecvData_Demo1
*   功能说明: CAN数据转发实验例程
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void CAN_RecvData_Demo1(void)
{
    u32 id;
    CAN_Id_TypeDef ide;
    CAN_RTR_TypeDef rtr;
    u8 data[8];
    u8 dlc,i;
    
    CAN_Receive();//读取缓存中的数据
    
    id=CAN_GetReceivedId();//读取ID号
    ide=CAN_GetReceivedIDE();//读取帧类型(标准或扩展)
    rtr=CAN_GetReceivedRTR();//读取帧类型(数据或远程)
    dlc=CAN_GetReceivedDLC();//读取接收到的数据字节数
    
    for(i=0;i<dlc;i++)
    {
        data[i]=CAN_GetReceivedData(i);//把接收到的数据送到自定义数组里    
    }
    
    CAN_Transmit(id,ide,rtr,dlc,data);//把接收到的数据转发出去(包括ID号、帧类型和数据)
}

void BSP_Configuration(void)
{  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//时钟速度为内部16M，1分频，
  CAN_Configuration();//调用CAN初始化函数
  rim();//打开总中断
  CAN_IER = 0x02;  //打开CAN总线接受中断
}

void beCAN_gpio_init()//初始化CAN总线IO口
{
  PG_DDR = 0x01; //PG0是CAN_TX设置为输出
  PG_CR1 = 0x01; //PG0输出为推挽
  PG_CR2 = 0x00; //PG0输出速度2M
  /*PG1是CAN_RX 默认是输入模式*/
}

/*CAN数据发送函数*/
void beCAN_TX(unsigned char data1,unsigned char data2,unsigned char data3,
              unsigned char data4,unsigned char data5,unsigned char data6,
              unsigned char data7,unsigned char data8)
{
  /*我们选择发送邮箱0*/
  CAN_FPSR = 0x00;//选择PAGE 0页 发送邮箱0
  /*CAN总线ID设置*/
  CAN_P2 = 0x47; //CAN_MIDR1 EXID[28:24]=   0 0111  //CAN_P2不能乱写ID，因为ID里面还包含了控制位，乱写会出现接受乱码
  CAN_P3 = 0xec;//CAN_MIDR2 EXID[23:16]=1110 1100
  CAN_P4 = 0x00;//CAN_MIDR3 EXID[15:8]=0000 0000
  CAN_P5 = 0x00;//CAN_MIDR3 EXID[7:0]=0000 0000
  
  CAN_P1 = 0x08;//CAN_MDLCR CAN发送8个数据
  
  /*把数据放入8个寄存器*/
  CAN_P6 = data1;
  CAN_P7 = data2;
  CAN_P8 = data3;
  CAN_P9 = data4;
  CAN_PA = data5;
  CAN_PB = data6;
  CAN_PC = data7;
  CAN_PD = data8;
  
  CAN_P0 |= 0x01;//把邮箱0数据发送出去 TXRQ=1
  Delay_us(100);
}

/*CAN数据发送函数接口封装*/
void beCAN_TX_package(unsigned long ID,
                     unsigned char data1,unsigned char data2,unsigned char data3,
                     unsigned char data4,unsigned char data5,unsigned char data6,
                     unsigned char data7,unsigned char data8)
{
  unsigned char ID1=0;
  unsigned char ID2=0;
  unsigned char ID3=0;
  unsigned char ID4=0;
  
  ID1 = (unsigned char )(((ID>>24)&0x4f)|0x40);
  ID2 = (unsigned char )(ID>>16);
  ID3 = (unsigned char )(ID>>8);
  ID4 = (unsigned char )(ID); 
  
  /*我们选择发送邮箱0*/
  CAN_FPSR = 0x00;//选择PAGE 0页 发送邮箱0
  /*CAN总线ID设置*/
  CAN_P2 = ID1; //CAN_MIDR1 EXID[28:24] //CAN_P2不能乱写ID，因为ID里面还包含了控制位，乱写会出现接受乱码
  CAN_P3 = ID2;//CAN_MIDR2 EXID[23:16]
  CAN_P4 = ID3;//CAN_MIDR3 EXID[15:8]
  CAN_P5 = ID4;//CAN_MIDR3 EXID[7:0]
  
  CAN_P1 = 0x08;//CAN_MDLCR CAN发送8个数据
  
  /*把数据放入8个寄存器*/
  CAN_P6 = data1;
  CAN_P7 = data2;
  CAN_P8 = data3;
  CAN_P9 = data4;
  CAN_PA = data5;
  CAN_PB = data6;
  CAN_PC = data7;
  CAN_PD = data8;
  
  CAN_P0 |= 0x01;//把邮箱0数据发送出去 TXRQ=1
  
  Delay_us(100);
}

void beCAN_filter_init_package(unsigned long ID)//设置接收过滤器值接口封装
{
  
  unsigned char FxR1,FxR2,FxR3,FxR4;
  CAN_FPSR = 0x06;//设置完过滤器，切换到PAGE 6页去激活过滤器
  CAN_PA |= 0x00;//关闭过滤器1
  /*因为滤波器寄存器在PAGE 2上，所以要先换页*/
  CAN_FPSR = 0x02;//PAGE 2上有过滤器0 过滤器1
 
  FxR1 = (unsigned char)(((ID>>24)&0x0f)<<3);
  FxR1 = (unsigned char)((((ID>>16)>>5)&0x07)|FxR1);
  FxR2 = (unsigned char)(((ID>>16)<<1)&0xe7);
  FxR2 = (unsigned char)((((ID>>8)>>7)&0x01)|FxR2);
  FxR3 = (unsigned char)((ID>>8)<<1);
  FxR3 = (unsigned char)(((ID>>7)&(0x01))|FxR3);
  FxR4 = (unsigned char)((ID<<1)&0xfe);
  
  CAN_P0 = FxR1;
  CAN_P1 = FxR2;
  CAN_P2 = FxR3;
  CAN_P3 = FxR4;
  
  
  CAN_P4 = 0x0f; //0000 1111 
  CAN_P5 = 0x00;//0000 0000
  CAN_P6 = 0xff; //1111 1111
  CAN_P7 = 0xff; //1111 1111
  
  CAN_FPSR = 0x06;//设置完过滤器，切换到PAGE 6页去激活过滤器
  CAN_PA |= 0x01;//激活过滤器1
  
}

void beCAN_RX(struct CAN_RX_buf *rxbuf)
{
  
  if(CAN_RFR==0x01) //表示CAN总线收到数据了
  {
    /*FIFO1 在PAGE7 页面*/
     CAN_FPSR = 0x07;//切换到页面7
     Delay_us(100);
     rxbuf->ID[0]=CAN_P2;
     rxbuf->ID[1]=CAN_P3;
     rxbuf->ID[2]=CAN_P4;
     rxbuf->ID[3]=CAN_P5;
     rxbuf->data[0]=CAN_P6;
     rxbuf->data[1]=CAN_P7;
     rxbuf->data[2]=CAN_P8;
     rxbuf->data[3]=CAN_P9;
     rxbuf->data[4]=CAN_PA;
     rxbuf->data[5]=CAN_PB;
     rxbuf->data[6]=CAN_PC;
     rxbuf->data[7]=CAN_PD;
     Delay_us(100);
     
     CAN_RFR |=0x20;//释放邮箱
  }
}

struct CAN_RX_buf rx_buf;

int main( void )
{
    beCAN_gpio_init();
    BSP_Configuration();//硬件驱动初始化函数
    beCAN_filter_init_package(0x0000ffff);
    UART1_init();
    while(1)
    {
      //beCAN_TX(0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88);
       
      Delay_ms(1000);
       
    }
    return 0;
}

INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  unsigned long ID,ID1;
  beCAN_RX(&rx_buf);
  
  ID1 = (unsigned long)rx_buf.ID[0];
  ID = ID1<<24;
  ID1 =(unsigned long)rx_buf.ID[1];
  ID = ID|(ID1<<16);
  ID1 =(unsigned long)rx_buf.ID[2];
  ID = ID|(ID1<<8);
  ID = ID|((unsigned long)rx_buf.ID[3]);
   
   
  beCAN_TX_package(ID,rx_buf.data[0],rx_buf.data[1],rx_buf.data[2],rx_buf.data[3],
                   rx_buf.data[4],rx_buf.data[5],rx_buf.data[6],rx_buf.data[7]);
  
}







