#ifndef _LED_H
#define _LED_H

#include "system.h"

#define LED_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_GPIO_PORT            GPIOB
#define LED_PIN                  GPIO_PIN_1

void led_init(void);
#define LED_TOGGLE()    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)
#define LED_ON()        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF()       HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)

#endif