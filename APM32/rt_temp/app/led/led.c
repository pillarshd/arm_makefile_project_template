#include "led.h"

/* GPIO PORT */
static GPIO_T* GPIO_PORT[LEDn] = {
#ifdef LED1_EN
    LED1_GPIO_PORT,
#endif
#ifdef LED2_EN
    LED2_GPIO_PORT
#endif
};
/* GPIO PIN */
static const GPIO_PIN_T GPIO_PIN[LEDn] = {
#ifdef LED1_EN
    LED1_PIN,
#endif
#ifdef LED2_EN
    LED2_PIN
#endif
};
/* GPIO CLOCK */
static const RCM_APB2_PERIPH_T GPIO_CLK[LEDn] = {
#ifdef LED1_EN
    LED1_GPIO_CLK,
#endif
#ifdef LED2_EN
    LED2_GPIO_CLK
#endif
};

static void led_init_internal(LedType Led)
{
    GPIO_Config_T  configStruct;

    /* Enable the GPIO_LED Clock */
    RCM_EnableAPB2PeriphClock(GPIO_CLK[Led]);

    /* Configure the GPIO_LED pin */
    configStruct.pin = GPIO_PIN[Led];
    configStruct.mode = GPIO_MODE_OUT_PP;
    configStruct.speed = GPIO_SPEED_50MHz;

    GPIO_Config(GPIO_PORT[Led], &configStruct);
    GPIO_PORT[Led]->BC = GPIO_PIN[Led];
}

void led_init()
{
    u8 i = LEDn;
    while (i--)
        led_init_internal((LedType)i);
}

void led_on(LedType Led)
{
    GPIO_PORT[Led]->BC |= GPIO_PIN[Led];
}

void led_off(LedType Led)
{
    GPIO_PORT[Led]->BSC |= GPIO_PIN[Led];
}

void led_toggle(LedType Led)
{
    GPIO_PORT[Led]->ODATA ^= GPIO_PIN[Led];
}
