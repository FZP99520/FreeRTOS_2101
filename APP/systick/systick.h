#ifndef _systick_H
#define _systick_H

#include "stm32f10x.h"


void SysTick_Init(u8 u8Sysclk);
void SysTick_delay_us(u32 u32delay_us);
void SysTick_delay_ms(u32 u32delay_ms);
void SysTick_os_delay_ms(u32 u32delay_ms);




#endif
