#include<iostm8s208rb.h>
#include<stdio.h>
#include<sys.h>
#include<uart.h>

struct CAN_RX_buf
{
    unsigned char ID[4];
    unsigned char data[8];
};

void beCAN_gpio_init();//初始化CAN总线IO口
void beCAN_bus_init();//初始化CAN总线
void beCAN_filter_init();//设置接收过滤器值
void beCAN_filter_init_package(unsigned long ID);//设置接收过滤器值接口封装
/*CAN数据发送函数接口封装*/
void beCAN_TX_package(unsigned long ID,
                     unsigned char data1,unsigned char data2,unsigned char data3,
                     unsigned char data4,unsigned char data5,unsigned char data6,
                     unsigned char data7,unsigned char data8);
/*CAN数据发送函数*/
void beCAN_TX(unsigned char data1,unsigned char data2,unsigned char data3,
              unsigned char data4,unsigned char data5,unsigned char data6,
              unsigned char data7,unsigned char data8);

/*CAN数据接受函数*/
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
    printf("can总线发送数据 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 \r\n");
   // beCAN_TX_package(1,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88);
   
    beCAN_TX(0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88);
   // beCAN_RX(&rx_buf);
    printf("\r\n");
    
    
    printf("can总线接受数据 ID = %x,%x,%x,%x,DATA = %x,%x,%x,%x,%x,%x,%x,%x\r\n",
           rx_buf.ID[0],rx_buf.ID[1],rx_buf.ID[2],rx_buf.ID[3],rx_buf.data[0],
           rx_buf.data[1],rx_buf.data[2],rx_buf.data[3],rx_buf.data[4],
           rx_buf.data[5],rx_buf.data[6],rx_buf.data[7]);
    printf("\r\n");
    Delay_ms(500);
    
  }
  
  
}

void beCAN_gpio_init()//初始化CAN总线IO口
{
  PG_DDR = 0x01; //PG0是CAN_TX设置为输出
  PG_CR1 = 0x01; //PG0输出为推挽
  PG_CR2 = 0x00; //PG0输出速度2M
  /*PG1是CAN_RX 默认是输入模式*/
}

void beCAN_bus_init()//初始化CAN总线
{
  CAN_MCR = 0x01; //CAN进入初始化模式
  while((CAN_MSR&0X01)==0X00);//等待CAN进入初始化模式
  Delay_us(100);
  CAN_MCR |= 0x50; //初始化模式设置
 // CAN_DGR = 0x01;  //设置CAN为环回模式
  CAN_DGR = 0x00;  //取消环回模式，设置成正常模式
  
  /*设置CAN总线的波特率100K*/
  /*设置页面里面的寄存器就不能直接向寄存器里面写值，要先换页再写值*/
 // CAN_FPSR = 0x06;//BTR1和BTR2寄存器是在PAGE 6,所以我用FPSR来切换页面到0x06
 // CAN_P4 = 0x05; // 同步跳跃 CAN_BTR1 BRP=5
 // CAN_P5 = 0x75; // CAN_BTR2 BS1=6 BS2=8  
  
  
  /*设置CAN总线的波特率125K*/
  /*设置页面里面的寄存器就不能直接向寄存器里面写值，要先换页再写值*/
  CAN_FPSR = 0x06;//BTR1和BTR2寄存器是在PAGE 6,所以我用FPSR来切换页面到0x06
  CAN_P4 = 0x04; // 同步跳跃 CAN_BTR1 BRP=4
  CAN_P5 = 0x75; // CAN_BTR2 BS1=6 BS2=8 
  
  /*设置过滤器*/
  CAN_P8 = 0x00;//CAN_FMR1 过滤器组0工作在屏蔽位
  CAN_P9 = 0x00;
  CAN_PA = 0x06;//过滤器组0 32位 过滤器组1 8位
  CAN_PB = 0x00;//过滤器组2 8位 过滤器组3 8位
  CAN_PC = 0x00;//过滤器组4 8位 过滤器组5 8位
  Delay_us(100);
  
  CAN_MCR &= 0xFE; //把MCR最低位INRQ=0 CAN进入正常模式
  //CAN_IER = 0x02;  //打开CAN总线接受中断
}

void beCAN_filter_init()//设置接收过滤器值
{
  /*因为滤波器寄存器在PAGE 2上，所以要先换页*/
  CAN_FPSR = 0x02;//PAGE 2上有过滤器0 过滤器1
  CAN_P0 = 0x3f;
  CAN_P1 = 0x00;
  CAN_P2 = 0x00;
  CAN_P3 = 0xfe;
  
  CAN_P4 = 0x3f;
  CAN_P5 = 0x00;
  CAN_P6 = 0x00;
  CAN_P7 = 0x00;
  
  CAN_FPSR = 0x06;//设置完过滤器，切换到PAGE 6页去激活过滤器
  CAN_PA |= 0x01;//激活过滤器1
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

/*CAN数据接受函数*/
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


#pragma vector=beCAN_FMP_vector
__interrupt void CAN()
{
  printf("进入CAN接受中断");
  beCAN_RX(&rx_buf); 
  CAN_RFR=(1<<3);
}


  


