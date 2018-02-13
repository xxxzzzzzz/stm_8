#include "pbdata.h"
/*********************************************************************************
*   函 数 名: LED_Init
*   功能说明: LED的GPIO管脚初始化
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void LED_Init(void)
{
  GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);//初始化LED1管脚为低速开漏输出高阻态
  GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);//初始化LED2管脚为低速开漏输出高阻态
  GPIO_Init(LED3_PORT, LED3_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);//初始化LED3管脚为低速开漏输出高阻态
}
/*********************************************************************************
*   函 数 名: LED_Demo1
*   功能说明: 闪烁例程，间隔时间为1秒。
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void LED_Demo1(void)
{
    LED1_L;//拉低PE0引脚，LED1发光二极管(发光)
    LED2_L;//拉低PA3引脚，LED2发光二极管(发光)
    LED3_L;//拉低PA6引脚，LED3发光二极管(发光)
    delay_ms(1000);//延时1秒
    
    LED1_H;//拉高PE0引脚，LED1发光二极管(熄灭)
    LED2_H;//拉高PA3引脚，LED2发光二极管(熄灭)
    LED3_H;//拉高PA6引脚，LED3发光二极管(熄灭)
    delay_ms(1000);//延时1秒
}
/*********************************************************************************
*   函 数 名: LED_Demo2
*   功能说明: 闪烁例程，间隔时间为1秒。
*   形    参：无
*   返 回 值: 无
*********************************************************************************/
void LED_Demo2(void)
{
    LED1_R;//PE0引脚电平取反，高电平时LED1发光二极管(熄灭)，底电平时LED1发光二极管(发光)
    LED2_R;//PA3引脚电平取反，高电平时LED2发光二极管(熄灭)，底电平时LED2发光二极管(发光)
    LED3_R;//PA6引脚电平取反，高电平时LED3发光二极管(熄灭)，底电平时LED3发光二极管(发光)
    delay_ms(1000);//延时1秒
}

/************************ 论坛地址 www.zxkjmcu.com ******************************/