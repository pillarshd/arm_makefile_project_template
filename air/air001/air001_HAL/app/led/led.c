#include "led.h"

void led_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    LED_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    /* GPIO初始化 */
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}
