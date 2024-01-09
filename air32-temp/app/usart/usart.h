#ifndef USART_H
#define USART_H

#include "system.h"
#include <stdio.h>

#define USART1_CLK                    RCC_APB2Periph_USART1 
#define USART1_TX_PIN                 GPIO_Pin_9
#define USART1_RX_PIN                 GPIO_Pin_10
#define USART1_GPIO_PORT              GPIOA
#define USART1_GPIO_CLK               RCC_APB2Periph_GPIOA

void usart1_init(u32 bound);

#endif
