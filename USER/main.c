#include "stm32f10x.h"
#include "led.h"
#include "clock.h"
#include "systick.h"
#include "usart.h"
#include "printf.h"
#include "pwm.h"

#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO  1
#define START_STK_SIZE   128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define LED_TASK_PRIO 2
#define LED_STK_SIZE  50
TaskHandle_t LedTask_Handler;
void Led_task(void *pvParameters);


int main()
{
    Clock_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_Init(72);
    Led_Init();

    xTaskCreate((TaskFunction_t)start_task,
        (const char *)"start_task",
        (uint16_t)START_STK_SIZE,
        NULL,
        (UBaseType_t)START_TASK_PRIO,
        (TaskHandle_t *)StartTask_Handler);
    vTaskStartScheduler();
    /*while(1)
	{
        //GPIO_ResetBits(GPIOB,GPIO_Pin_All);
        LED_BLUE_ON;
        SysTick_delay_ms(1000);
        LED_BLUE_OFF;
        //GPIO_SetBits(GPIOB,GPIO_Pin_All);
        SysTick_delay_ms(1000);
	}*/
}


void start_task(void * pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)Led_task,
        (const char *)"led_task",
        (uint16_t)LED_STK_SIZE,
        NULL,
        (UBaseType_t)LED_TASK_PRIO,
        (TaskHandle_t *)LedTask_Handler);
    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();
}

void Led_task(void * pvParameters)
{
    while(1)
    {
        LED_BLUE_ON;
        SysTick_os_delay_ms(200);
        LED_BLUE_OFF;
        SysTick_os_delay_ms(800);
    }
}


