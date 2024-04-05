#ifndef _LED_H
#define _LED_H

#include "system.h"

#define LED_GPIO_CLK        LL_IOP_GRP1_PERIPH_GPIOA 
#define LED_GPIO_PORT       GPIOB
#define LED_PIN             LL_GPIO_PIN_1

void led_init(void);
#define LED_TOGGLE()    LL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)
#define LED_ON()        LL_GPIO_ResetOutputPin(LED_GPIO_PORT, LED_PIN)
#define LED_OFF()       LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED_PIN)

#endif