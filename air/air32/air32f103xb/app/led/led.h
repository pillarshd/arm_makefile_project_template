#ifndef LED_H
#define LED_H

#include "system.h"

#define LED1_PIN                         GPIO_Pin_13
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOC 
#define LED1_OUT    PCout(13)

void led1_init(void);

#define LED_TURN \
    do{                         \
        LED1_OUT = !LED1_OUT;   \
    }while(0)

#endif
