#include "pbdata.h"
/*********************************************************************************
*   函 数 名: CAN_Configuration
*   功能说明: CAN初始化
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void CAN_Configuration(void)
{
    //初始化CAN波特率为1M  
    CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_11TimeQuantum,CAN_BitSeg2_4TimeQuantum,1);
    //配置CAN第0组过滤器为屏蔽模式。但是屏蔽位都设置为0，所以相当于没启动屏蔽功能。
    CAN_FilterInit(CAN_FilterNumber_0,ENABLE,CAN_FilterMode_IdMask,CAN_FilterScale_32Bit,0,0,0,0,0,0,0,0);
    //使能CAN接收中断功能
    CAN_ITConfig(CAN_IT_FMP,ENABLE);
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