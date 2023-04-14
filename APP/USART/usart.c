#include "usart.h"
void Usart_Init()   //串口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef  NVIC_InitStructure;//中断优先级
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//串口时钟
	/*********************管脚配置***************************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//txd
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//rxd
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*******************************************************/
	/***************串口配置******************/
	USART_InitStructure.USART_BaudRate=9600;//波特率9600
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位--无
	USART_InitStructure.USART_HardwareFlowControl = //硬件流失能
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;//模式
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE); //串口使能
	/*************************************/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收中断使能
	USART_ClearFlag(USART1,USART_FLAG_TC);//清空发送标志位
	/***************配置优先级********************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //分组
    NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn; //通道选择
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //响应优先级 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能
    NVIC_Init(&NVIC_InitStructure); //初始化
}

