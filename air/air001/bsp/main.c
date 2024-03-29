#include "system.h"

static void APP_GpioConfig(void);

int main(void)
{
    /* 初始化所有外设，Flash接口，SysTick */
    HAL_Init();
    /* 初始化GPIO */
    APP_GpioConfig();
    set_system_clock();

    while (1)
    {
        HAL_Delay(250);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    }
}

static void APP_GpioConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE(); /* 使能GPIOB时钟 */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   /* 推挽输出 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;           /* 使能上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* GPIO速度 */
    /* GPIO初始化 */
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
