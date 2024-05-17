#ifndef LED_H
#define LED_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEDn                             1U
#if LEDn >= 1u
#define LED1_EN
#endif
#if LEDn >= 2u
#define LED2_EN
#endif

typedef enum
{
#ifdef LED1_EN
    LED1 = 0,
#endif
#ifdef LED2_EN
    LED2,
#endif
} LedType;


#ifdef LED1_EN
#define LED1_PIN                         GPIO_PIN_13
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCM_APB2_PERIPH_GPIOC
#define LED1_OUT    PCOUT(13)
#endif

#ifdef LED2_EN
#define LED2_PIN                         GPIO_PIN_0
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCU_GPIOC
#define LED2_OUT    PCOUT(0)
#endif

void led_init(void);
void led_on(LedType Led); // turn on selected led
void led_off(LedType Led); // turn off selected led
void led_toggle(LedType Led); // toggle the selected led

#ifdef __cplusplus
}
#endif

#endif
