#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"


static u8 _u8fac_us = 0;
static u16 _u16fac_ms = 0;

void SysTick_Init(u8 u8Sysclk)
{
    u32 u32Reload = 0;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    _u8fac_us = SystemCoreClock/1000000;
    u32Reload = SystemCoreClock/1000000;
    u32Reload *= 1000000/configTICK_RATE_HZ;

    _u16fac_ms = 1000/configTICK_RATE_HZ;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_delay_us(u32 u32delay_us)
{
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload = SysTick->LOAD;
    ticks = u32delay_us*_u8fac_us;
    told = SysTick->VAL;
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow<told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if(tcnt > ticks) break;
        }
    }
}

void SysTick_delay_ms(u32 u32delay_ms)
{
    u32 u32temp = 0;
    for(u32temp = 0;u32temp < u32delay_ms; u32temp++)
    {
        SysTick_delay_us(1000);
    }
}

void SysTick_os_delay_ms(u32 u32delay_ms)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        if(u32delay_ms >= _u16fac_ms)
        {
            vTaskDelay(u32delay_ms/_u16fac_ms);
        }
        u32delay_ms %= _u16fac_ms;
        SysTick_delay_us((u32)(u32delay_ms*1000));
    }
}

/*
void Delay_ms(u32 x)
{
	 u32 temp;
	SysTick->CTRL=0x01;
	SysTick->LOAD=9000*x;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->VAL=0;
	SysTick->CTRL=0;
}
void Delay_us(u32 x)
{
	 u32 temp;
	SysTick->CTRL=0x01;
	SysTick->LOAD=9*x;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->VAL=0;
	SysTick->CTRL=0;
}
*/

