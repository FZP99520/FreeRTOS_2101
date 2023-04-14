#include "stm32f10x.h"
#include "led.h"
#include "clock.h"
#include "systick.h"
#include "usart.h"
#include "printf.h"
#include "pwm.h"


int main()
{
    Clock_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_Init(72);
	Led_Init();
	//PHASE_PWM_Init(3600-1,0);
	//TIM1_Int_Init(1200-1,0);
	//pp.pwm1=800;
	//pp.pwm2=800;
	//pp.pwm3=800;
	//PWM_Set(pp);
	while(1)
	{
        //GPIO_ResetBits(GPIOB,GPIO_Pin_All);
        LED_BLUE_ON;
        SysTick_delay_ms(1000);
        LED_BLUE_OFF;
        //GPIO_SetBits(GPIOB,GPIO_Pin_All);
        SysTick_delay_ms(1000);
	}
}
