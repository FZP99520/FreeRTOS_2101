#include "led.h"
void Led_Init()  //LED初始化函数
{
    GPIO_InitTypeDef  GPIO_InitStruct; //结构体变量定义
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    /**********************************/
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    //GPIO_SetBits(GPIOA,LED_Pin);
}


