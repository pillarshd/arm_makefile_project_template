#include "system.h"

void HAL_MspInit(void)
{
    /* system_init--HSI 48M */
    set_system_clock();
    LOG_INIT(115200);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
#if (LOG_ON != 0)
    if(huart->Instance == LOG_USART)
    {
        LOG_RX_GPIO_CLK_ENABLE();
        LOG_TX_GPIO_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = LOG_TX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = LOG_TX_AF;
        HAL_GPIO_Init(LOG_TX_GPIO_PORT, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = LOG_RX_PIN;
        GPIO_InitStruct.Alternate = LOG_RX_AF;
        HAL_GPIO_Init(LOG_RX_GPIO_PORT, &GPIO_InitStruct);
    }
#endif
}
