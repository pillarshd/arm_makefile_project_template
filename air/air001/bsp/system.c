#include "system.h"

static void set_system_clock_HSI_48Hz()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    /* 配置时钟源HSE/HSI/LSE/LSI */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | 
                                       RCC_OSCILLATORTYPE_HSI | 
                                       RCC_OSCILLATORTYPE_LSI | 
                                       RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* 开启HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* 不分频 */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* 配置HSI输出时钟为24MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* 关闭HSE */
    RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;                     /* HSE工作频率范围16M~32M */
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* 关闭LSI */
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* 关闭LSE */
    RCC_OscInitStruct.LSEDriver = RCC_ECSCR_LSE_DRIVER_1;             /* LSE默认驱动能力 */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                      /* 开启PLL */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;              /* PLL的时钟源，频率必须要求12MHz及以上 */
    /* 初始化RCC振荡器 */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    /*初始化CPU,AHB,APB总线时钟*/
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 配置PLL为系统时钟 */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* APH时钟不分频 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* APB时钟不分频 */
    /* 初始化RCC系统时钟 */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
        Error_Handler();
}


void set_system_clock()
{
    // HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
    set_system_clock_HSI_48Hz();
}

#if (LOG_ON != 0)

UART_HandleTypeDef log_uart_handle;
void log_init(u32 baud)
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

    LOG_CLK_ENABLE();
    log_uart_handle.Instance = LOG_USART;
    log_uart_handle.Init.BaudRate = baud;
    log_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    log_uart_handle.Init.StopBits = UART_STOPBITS_1;
    log_uart_handle.Init.Parity = UART_PARITY_NONE;
    log_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    log_uart_handle.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&log_uart_handle);
}

int _write(int fd, char *ptr, int size)
{
    HAL_UART_Transmit(&log_uart_handle, (u8 *)ptr, size, HAL_MAX_DELAY);
    return size;
}

#endif
