#include<iostm8s208rb.h>
#include<stdio.h>
#include<sys.h>
#include<uart.h>

struct CAN_RX_buf
{
    unsigned char ID[4];
    unsigned char data[8];
};

void beCAN_gpio_init();//��ʼ��CAN����IO��
void beCAN_bus_init();//��ʼ��CAN����
void beCAN_filter_init();//���ý��չ�����ֵ
void beCAN_filter_init_package(unsigned long ID);//���ý��չ�����ֵ�ӿڷ�װ
/*CAN���ݷ��ͺ����ӿڷ�װ*/
void beCAN_TX_package(unsigned long ID,
                     unsigned char data1,unsigned char data2,unsigned char data3,
                     unsigned char data4,unsigned char data5,unsigned char data6,
                     unsigned char data7,unsigned char data8);
/*CAN���ݷ��ͺ���*/
void beCAN_TX(unsigned char data1,unsigned char data2,unsigned char data3,
              unsigned char data4,unsigned char data5,unsigned char data6,
              unsigned char data7,unsigned char data8);

/*CAN���ݽ��ܺ���*/
void beCAN_RX(struct CAN_RX_buf *rxbuf);
struct CAN_RX_buf rx_buf;
int main( void )
{
  
  
  CLK_start();
  UART1_init();
  beCAN_gpio_init();
  beCAN_bus_init();
  //beCAN_filter_init();
 // beCAN_filter_init_package(0x47ec0000);
  rx_buf.ID[0]=0x00;
  rx_buf.ID[1]=0x00;
  rx_buf.ID[2]=0x00;
  rx_buf.ID[3]=0x00;
  rx_buf.data[0]=0x00;
  rx_buf.data[1]=0x00;
  rx_buf.data[2]=0x00;
  rx_buf.data[3]=0x00;
  rx_buf.data[4]=0x00;
  rx_buf.data[5]=0x00;
  rx_buf.data[6]=0x00;
  rx_buf.data[7]=0x00;
  
  
  
  
  while(1)
  {
    printf("can���߷������� 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 \r\n");
   // beCAN_TX_package(1,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88);
   
    beCAN_TX(0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88);
   // beCAN_RX(&rx_buf);
    printf("\r\n");
    
    
    printf("can���߽������� ID = %x,%x,%x,%x,DATA = %x,%x,%x,%x,%x,%x,%x,%x\r\n",
           rx_buf.ID[0],rx_buf.ID[1],rx_buf.ID[2],rx_buf.ID[3],rx_buf.data[0],
           rx_buf.data[1],rx_buf.data[2],rx_buf.data[3],rx_buf.data[4],
           rx_buf.data[5],rx_buf.data[6],rx_buf.data[7]);
    printf("\r\n");
    Delay_ms(500);
    
  }
  
  
}

void beCAN_gpio_init()//��ʼ��CAN����IO��
{
  PG_DDR = 0x01; //PG0��CAN_TX����Ϊ���
  PG_CR1 = 0x01; //PG0���Ϊ����
  PG_CR2 = 0x00; //PG0����ٶ�2M
  /*PG1��CAN_RX Ĭ��������ģʽ*/
}

void beCAN_bus_init()//��ʼ��CAN����
{
  CAN_MCR = 0x01; //CAN�����ʼ��ģʽ
  while((CAN_MSR&0X01)==0X00);//�ȴ�CAN�����ʼ��ģʽ
  Delay_us(100);
  CAN_MCR |= 0x50; //��ʼ��ģʽ����
 // CAN_DGR = 0x01;  //����CANΪ����ģʽ
  CAN_DGR = 0x00;  //ȡ������ģʽ�����ó�����ģʽ
  
  /*����CAN���ߵĲ�����100K*/
  /*����ҳ������ļĴ����Ͳ���ֱ����Ĵ�������дֵ��Ҫ�Ȼ�ҳ��дֵ*/
 // CAN_FPSR = 0x06;//BTR1��BTR2�Ĵ�������PAGE 6,��������FPSR���л�ҳ�浽0x06
 // CAN_P4 = 0x05; // ͬ����Ծ CAN_BTR1 BRP=5
 // CAN_P5 = 0x75; // CAN_BTR2 BS1=6 BS2=8  
  
  
  /*����CAN���ߵĲ�����125K*/
  /*����ҳ������ļĴ����Ͳ���ֱ����Ĵ�������дֵ��Ҫ�Ȼ�ҳ��дֵ*/
  CAN_FPSR = 0x06;//BTR1��BTR2�Ĵ�������PAGE 6,��������FPSR���л�ҳ�浽0x06
  CAN_P4 = 0x04; // ͬ����Ծ CAN_BTR1 BRP=4
  CAN_P5 = 0x75; // CAN_BTR2 BS1=6 BS2=8 
  
  /*���ù�����*/
  CAN_P8 = 0x00;//CAN_FMR1 ��������0����������λ
  CAN_P9 = 0x00;
  CAN_PA = 0x06;//��������0 32λ ��������1 8λ
  CAN_PB = 0x00;//��������2 8λ ��������3 8λ
  CAN_PC = 0x00;//��������4 8λ ��������5 8λ
  Delay_us(100);
  
  CAN_MCR &= 0xFE; //��MCR���λINRQ=0 CAN��������ģʽ
  //CAN_IER = 0x02;  //��CAN���߽����ж�
}

void beCAN_filter_init()//���ý��չ�����ֵ
{
  /*��Ϊ�˲����Ĵ�����PAGE 2�ϣ�����Ҫ�Ȼ�ҳ*/
  CAN_FPSR = 0x02;//PAGE 2���й�����0 ������1
  CAN_P0 = 0x3f;
  CAN_P1 = 0x00;
  CAN_P2 = 0x00;
  CAN_P3 = 0xfe;
  
  CAN_P4 = 0x3f;
  CAN_P5 = 0x00;
  CAN_P6 = 0x00;
  CAN_P7 = 0x00;
  
  CAN_FPSR = 0x06;//��������������л���PAGE 6ҳȥ���������
  CAN_PA |= 0x01;//���������1
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

/*CAN���ݽ��ܺ���*/
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


#pragma vector=beCAN_FMP_vector
__interrupt void CAN()
{
  printf("����CAN�����ж�");
  beCAN_RX(&rx_buf); 
  CAN_RFR=(1<<3);
}


  


