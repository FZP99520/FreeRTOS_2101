#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"
typedef struct
{
	u16 pwm1;
	u16 pwm2;
	u16 pwm3;
	u16 pwm4;
}PWM_TypeDef;
void PWM_Set(PWM_TypeDef p);
void PHASE_PWM_Init(u16 arr,u16 psr);
void TIM1_Int_Init(u16 arr,u16 psr);
#endif
