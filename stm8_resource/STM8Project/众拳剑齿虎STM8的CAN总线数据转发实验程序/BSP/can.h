#ifndef _CAN_H  //宏定义，定义文件名称
#define _CAN_H
/*---------------------------头文件引用--------------------------------------*/
#include "stm8s.h"//引用STM8头文件

/*---------------------------函数声明--------------------------------------*/
void CAN_Configuration(void);//CAN初始化函数
void CAN_RecvData_Demo1(void);//CAN数据转发函数

#endif //定义文件名称结束