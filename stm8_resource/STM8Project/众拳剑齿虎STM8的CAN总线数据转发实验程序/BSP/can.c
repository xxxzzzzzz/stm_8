#include "pbdata.h"
/*********************************************************************************
*   �� �� ��: CAN_Configuration
*   ����˵��: CAN��ʼ��
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************/
void CAN_Configuration(void)
{
    //��ʼ��CAN������Ϊ1M  
    CAN_Init(CAN_MasterCtrl_AllDisabled,CAN_Mode_Normal,CAN_SynJumpWidth_1TimeQuantum,CAN_BitSeg1_11TimeQuantum,CAN_BitSeg2_4TimeQuantum,1);
    //����CAN��0�������Ϊ����ģʽ����������λ������Ϊ0�������൱��û�������ι��ܡ�
    CAN_FilterInit(CAN_FilterNumber_0,ENABLE,CAN_FilterMode_IdMask,CAN_FilterScale_32Bit,0,0,0,0,0,0,0,0);
    //ʹ��CAN�����жϹ���
    CAN_ITConfig(CAN_IT_FMP,ENABLE);
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