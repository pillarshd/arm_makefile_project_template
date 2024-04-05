#include "system.h"

static void set_system_clock_HSI_32Hz()
{
    LL_RCC_HSI_Enable();
    LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_16MHz);
    while (LL_RCC_HSI_IsReady() != 1);

    LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct;
    UTILS_ClkInitStruct.AHBCLKDivider = LL_RCC_SYSCLK_DIV_1;
    UTILS_ClkInitStruct.APB1CLKDivider = LL_RCC_APB1_DIV_1;
    LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct);
    /* 设置AHB分频 */
    // LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    // /* 配置HSISYS为系统时钟及初始化 */
    // LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    // while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

    // /* 设置APB1分频及初始化 */
    // LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
    LL_SetSystemCoreClock(32000000);
}


void set_system_clock()
{
    // /* 配置PA1为复用模式，并复用为MCO输出引脚 */
    // LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    // LL_GPIO_InitTypeDef GPIO_InitStruct;
    // GPIO_InitStruct.Pin = LL_GPIO_PIN_1;/* 选择1号引脚 */
    // GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;/* 配置为复用模式 */
    // GPIO_InitStruct.Alternate = LL_GPIO_AF15_MCO;/* 选择复用为AF15功能 */
    // GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;/* 输出速度选择 */
    // GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;/* 输出模式选择 */
    // GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;/* 无上下拉 */
    // LL_GPIO_Init(GPIOA, &GPIO_InitStruct);/* 初始化GPIOA */
    // LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK,LL_RCC_MCO1_DIV_1);

    set_system_clock_HSI_32Hz();
}

#if (LOG_ON != 0)

void log_init(u32 baud)
{
    /*GPIO配置*/
    LL_IOP_GRP1_EnableClock(LOG_TX_GPIO_CLK);
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LOG_TX_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LOG_TX_AF;
    LL_GPIO_Init(LOG_TX_GPIO_PORT, &GPIO_InitStruct);
    // LL_IOP_GRP1_EnableClock(LOG_RX_GPIO_CLK);
    // GPIO_InitStruct.Pin = LOG_RX_PIN;
    // GPIO_InitStruct.Alternate = LOG_TX_AF;
    // LL_GPIO_Init(LOG_RX_GPIO_PORT, &GPIO_InitStruct);
    /*配置USART功能*/
    LL_APB1_GRP1_EnableClock(LOG_CLK);/*使能USART时钟*/
    LL_USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.BaudRate = baud;/*设置波特率*/
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;/*设置数据长度*/
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;/*停止位*/
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;/*设置校验位*/
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    /*USART初始化*/
    LL_USART_Init(LOG_USART, &USART_InitStruct);
    /*配置为全双工异步模式*/
    LL_USART_ConfigAsyncMode(LOG_USART);
    /*使能UART模块*/
    LL_USART_Enable(LOG_USART);
}

int _write(int fd, char *ptr, int size)
{
    int TxCount = size;
    /*发送数据*/
    while (TxCount > 0)
    {
        /* 等待TXE标志位置位 */
        while (LL_USART_IsActiveFlag_TXE(LOG_USART) != 1);
        /* 发送数据 */
        LL_USART_TransmitData8(LOG_USART, *ptr);
        ptr++;
        TxCount--;
    }
    /*等待TC标志位置位*/
    while (LL_USART_IsActiveFlag_TC(LOG_USART) != 1);
    return size;
}

#endif
