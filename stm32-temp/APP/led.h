#ifndef LED_H
#define LED_H

#include "system.h"
#include "bit_band.h"

#define LED1_PORT_RCC_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_PORT           GPIOB
#define LED1_PIN            GPIO_PIN_5
#define LED1_ON()           HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET)
#define LED1_OFF()          HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET)

#define LED1_OUT    PB5_OUT


#define LED2    1
#if LED2
#define LED2_PORT_RCC_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED2_PORT           GPIOE
#define LED2_PIN            GPIO_PIN_5
#define LED2_ON()           HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET)
#define LED2_OFF()          HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET)

#define LED2_OUT    PE5_OUT
#endif

void led_init(void);


#endif
