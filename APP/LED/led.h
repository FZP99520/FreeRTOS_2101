#ifndef _led_H
#define _led_H

#include "stm32f10x.h"

#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN  GPIO_Pin_12

#define LED_BLUE_ON     GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_BLUE_OFF    GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_BLUE_STATUS GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)

#if 0
#define LED_Red_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define LED_Red_OFF GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define LED_Red_STA GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)
#endif

void Led_Init(void);

#endif
