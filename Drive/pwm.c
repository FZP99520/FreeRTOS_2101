#include "pwm.h"
#include "misc.h"
static void Phase1_PWM_Init(u16 arr,u16 psr);
static void Phase2_PWM_Init(u16 arr,u16 psr);
static void Phase3_PWM_Init(u16 arr,u16 psr);
void PWM_Set(PWM_TypeDef p)
{
	TIM2->CCR2=p.pwm1;
	TIM3->CCR1=p.pwm2;
	TIM4->CCR1=p.pwm3;
}
void TIM1_Int_Init(u16 arr,u16 psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   

	TIM_TimeBaseInitStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psr;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
 NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
 NVIC_Init(&NVIC_InitStructure);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}
void PHASE_PWM_Init(u16 arr,u16 psr)
{
	Phase1_PWM_Init(arr,psr);
	Phase2_PWM_Init(arr,psr);
	Phase3_PWM_Init(arr,psr);
	
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
	TIM_Cmd(TIM4,DISABLE);
	TIM2->CNT=0;
	TIM3->CNT=arr/3*2;
	TIM4->CNT=arr/3;;
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}
void Phase1_PWM_Init(u16 arr,u16 psr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =psr;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=1000;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM2, ENABLE);
		
	TIM_Cmd(TIM2,ENABLE);
}
void Phase2_PWM_Init(u16 arr,u16 psr)//20kHz
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =psr;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=1000;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM3, ENABLE);
		
	TIM_Cmd(TIM3,ENABLE);
  //TIM_CtrlPWMOutputs(TIM1, ENABLE);//pwm输出  
	
}
void Phase3_PWM_Init(u16 arr,u16 psr)//20kHz
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler =psr;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=1000;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);
		
	TIM_Cmd(TIM4,ENABLE);
  //TIM_CtrlPWMOutputs(TIM1, ENABLE);//pwm输出  
}
