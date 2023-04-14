#include "stm32f10x.h"
#include "led.h"
#include "systick.h"
#include "usart.h"
#include "printf.h"
#include "pwm.h"
int main()
{
	u8 a;
	PWM_TypeDef pp;
	SystemInit();
	Delay_ms(100);
	Led_Init();
	PHASE_PWM_Init(3600-1,0);
	//TIM1_Int_Init(1200-1,0);
	pp.pwm1=800;
	pp.pwm2=800;
	pp.pwm3=800;
	PWM_Set(pp);
	while(1)
	{
		
	}
}
