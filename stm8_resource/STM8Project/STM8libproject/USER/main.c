#include<iostm8s208rb.h>
#include<uart.h>
#include "stm8s.h"//����STM8��ͷ�ļ�
#include "stm8s_can.h"
#include "stm8s_clk.h"
#include "stm8s_it.h"
struct CAN_RX_buf
{
    unsigned char ID[4];
    unsigned char data[8];
};

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


void CAN_FilterInit_socket(unsigned long ID)//CAN�̼���ӿڷ�װ
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
*   �� �� ��: CAN_Configuration
*   ����˵��: CAN��ʼ��
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************/
void CAN_Configuration(void)
{
    //��ʼ��CAN������Ϊ1M  
  /*CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_11TimeQuantum,
             CAN_BitSeg2_4TimeQuantum,1);*/
  //��ʼ��CAN������Ϊ800K  
 /* CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_5TimeQuantum,
             CAN_BitSeg2_4TimeQuantum,2);*/
  
  //��ʼ��CAN������Ϊ500K  
  /*CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,4);*/
  
  //��ʼ��CAN������Ϊ250K  
  CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,8);
  
  //��ʼ��CAN������Ϊ125K  
 /* CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_8TimeQuantum,
             CAN_BitSeg2_7TimeQuantum,8);*/
  
    //����CAN��0�������Ϊ����ģʽ����������λ������Ϊ0�������൱��û�������ι��ܡ�
  //CAN_FilterInit(CAN_FilterNumber_0,ENABLE,CAN_FilterMode_IdMask,CAN_FilterScale_32Bit,0,0,0x7f,0xff,0,0,0xff,0xff);
    CAN_FilterInit_socket(0x3efe);
    
    //ʹ��CAN�����жϹ���
    //CAN_ITConfig(CAN_IT_FMP,ENABLE);
    
   
}



/*********************************************************************************
*   �� �� ��: CAN_RecvData_Demo1
*   ����˵��: CAN����ת��ʵ������
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************/
void CAN_RecvData_Demo1(void)
{
    u32 id;
    CAN_Id_TypeDef ide;
    CAN_RTR_TypeDef rtr;
    u8 data[8];
    u8 dlc,i;
    
    CAN_Receive();//��ȡ�����е�����
    
    id=CAN_GetReceivedId();//��ȡID��
    ide=CAN_GetReceivedIDE();//��ȡ֡����(��׼����չ)
    rtr=CAN_GetReceivedRTR();//��ȡ֡����(���ݻ�Զ��)
    dlc=CAN_GetReceivedDLC();//��ȡ���յ��������ֽ���
    
    for(i=0;i<dlc;i++)
    {
        data[i]=CAN_GetReceivedData(i);//�ѽ��յ��������͵��Զ���������    
    }
    
    CAN_Transmit(id,ide,rtr,dlc,data);//�ѽ��յ�������ת����ȥ(����ID�š�֡���ͺ�����)
}

void BSP_Configuration(void)
{  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//ʱ���ٶ�Ϊ�ڲ�16M��1��Ƶ��
  CAN_Configuration();//����CAN��ʼ������
  rim();//�����ж�
  CAN_IER = 0x02;  //��CAN���߽����ж�
}

void beCAN_gpio_init()//��ʼ��CAN����IO��
{
  PG_DDR = 0x01; //PG0��CAN_TX����Ϊ���
  PG_CR1 = 0x01; //PG0���Ϊ����
  PG_CR2 = 0x00; //PG0����ٶ�2M
  /*PG1��CAN_RX Ĭ��������ģʽ*/
}

/*CAN���ݷ��ͺ���*/
void beCAN_TX(unsigned char data1,unsigned char data2,unsigned char data3,
              unsigned char data4,unsigned char data5,unsigned char data6,
              unsigned char data7,unsigned char data8)
{
  /*����ѡ��������0*/
  CAN_FPSR = 0x00;//ѡ��PAGE 0ҳ ��������0
  /*CAN����ID����*/
  CAN_P2 = 0x47; //CAN_MIDR1 EXID[28:24]=   0 0111  //CAN_P2������дID����ΪID���滹�����˿���λ����д����ֽ�������
  CAN_P3 = 0xec;//CAN_MIDR2 EXID[23:16]=1110 1100
  CAN_P4 = 0x00;//CAN_MIDR3 EXID[15:8]=0000 0000
  CAN_P5 = 0x00;//CAN_MIDR3 EXID[7:0]=0000 0000
  
  CAN_P1 = 0x08;//CAN_MDLCR CAN����8������
  
  /*�����ݷ���8���Ĵ���*/
  CAN_P6 = data1;
  CAN_P7 = data2;
  CAN_P8 = data3;
  CAN_P9 = data4;
  CAN_PA = data5;
  CAN_PB = data6;
  CAN_PC = data7;
  CAN_PD = data8;
  
  CAN_P0 |= 0x01;//������0���ݷ��ͳ�ȥ TXRQ=1
  Delay_us(100);
}

/*CAN���ݷ��ͺ����ӿڷ�װ*/
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
  
  /*����ѡ��������0*/
  CAN_FPSR = 0x00;//ѡ��PAGE 0ҳ ��������0
  /*CAN����ID����*/
  CAN_P2 = ID1; //CAN_MIDR1 EXID[28:24] //CAN_P2������дID����ΪID���滹�����˿���λ����д����ֽ�������
  CAN_P3 = ID2;//CAN_MIDR2 EXID[23:16]
  CAN_P4 = ID3;//CAN_MIDR3 EXID[15:8]
  CAN_P5 = ID4;//CAN_MIDR3 EXID[7:0]
  
  CAN_P1 = 0x08;//CAN_MDLCR CAN����8������
  
  /*�����ݷ���8���Ĵ���*/
  CAN_P6 = data1;
  CAN_P7 = data2;
  CAN_P8 = data3;
  CAN_P9 = data4;
  CAN_PA = data5;
  CAN_PB = data6;
  CAN_PC = data7;
  CAN_PD = data8;
  
  CAN_P0 |= 0x01;//������0���ݷ��ͳ�ȥ TXRQ=1
  
  Delay_us(100);
}

void beCAN_filter_init_package(unsigned long ID)//���ý��չ�����ֵ�ӿڷ�װ
{
  
  unsigned char FxR1,FxR2,FxR3,FxR4;
  CAN_FPSR = 0x06;//��������������л���PAGE 6ҳȥ���������
  CAN_PA |= 0x00;//�رչ�����1
  /*��Ϊ�˲����Ĵ�����PAGE 2�ϣ�����Ҫ�Ȼ�ҳ*/
  CAN_FPSR = 0x02;//PAGE 2���й�����0 ������1
 
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
  
  CAN_FPSR = 0x06;//��������������л���PAGE 6ҳȥ���������
  CAN_PA |= 0x01;//���������1
  
}

void beCAN_RX(struct CAN_RX_buf *rxbuf)
{
  
  if(CAN_RFR==0x01) //��ʾCAN�����յ�������
  {
    /*FIFO1 ��PAGE7 ҳ��*/
     CAN_FPSR = 0x07;//�л���ҳ��7
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
     
     CAN_RFR |=0x20;//�ͷ�����
  }
}

struct CAN_RX_buf rx_buf;

int main( void )
{
    beCAN_gpio_init();
    BSP_Configuration();//Ӳ��������ʼ������
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







