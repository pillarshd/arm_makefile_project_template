#include "system.h"

#if (LOG_ON != 0)

void log_init(u32 baud)
{   
   /* enable GPIO clock */
   RCM_EnableAPB2PeriphClock(LOG_GPIO_CLK);
   /* enable USART clock */
   RCM_EnableAPB2PeriphClock(LOG_CLK);
   /* connect port to USARTx_Tx */
   GPIO_Config_T gpio_conf;
   gpio_conf.mode = GPIO_MODE_AF_PP;
   gpio_conf.pin = LOG_TX_PIN;
   gpio_conf.speed = GPIO_SPEED_50MHz;
   GPIO_Config(LOG_GPIO_PORT, &gpio_conf);
   /* connect port to USARTx_Rx */
   gpio_conf.mode = GPIO_MODE_IN_FLOATING;
   gpio_conf.pin = LOG_RX_PIN;
   GPIO_Config(LOG_GPIO_PORT, &gpio_conf);
   /* USART configure */
   USART_Config_T usart1_conf;
   usart1_conf.baudRate = baud;
   usart1_conf.wordLength = USART_WORD_LEN_8B;
   usart1_conf.stopBits = USART_STOP_BIT_1;
   usart1_conf.parity = USART_PARITY_NONE;
   usart1_conf.mode = USART_MODE_TX_RX;
   usart1_conf.hardwareFlow = USART_HARDWARE_FLOW_NONE;
   USART_Config(LOG_USART, &usart1_conf);
   USART_Enable(LOG_USART);
}

int _write(int fd, char *ptr, int size)
{
    int n = size;
    while(n--)
    {
        while (RESET == USART_ReadStatusFlag(LOG_USART, USART_FLAG_TXBE));
        USART_TxData(LOG_USART, (u16)*ptr++);
    }
    while(RESET == USART_ReadStatusFlag(LOG_USART, USART_FLAG_TXC));
    return size;
}
#endif // (LOG_ON != 0)

static u8 fac_us = 0; // 1微秒要的步数
static u16 fac_ms = 0;
static u32 fac_s = 0;

static void systick_init()
{
    SysTick_Config(SystemCoreClock / 1000U);
    SysTick_ConfigCLKSource(SYSTICK_CLK_SOURCE_HCLK_DIV8);
    fac_s = SystemCoreClock / 8;
    fac_ms = fac_s / 1000;
    fac_us = fac_ms / 1000;
}

static void delay_internal(u32 n, u32 fac)
{
    u32 tcnt = 0;
    u32 ticks = n * fac;
    u32 tnow = 0;
    u32 told = SysTick->VAL;
    u32 reload = SysTick->LOAD;

    while(tcnt < ticks)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else // 溢出过
                tcnt += reload - tnow + told;
            told = tnow;
        }
    }
}

void delay_us(u32 nus)
{
    while(nus > 1000000)
    {
        nus -= 1000000;
        delay_internal(1000000, fac_us);
    }
    delay_internal(nus, fac_us);
}


void delay_ms(u16 nms)
{
    while(nms > 1000)
    {
        nms -= 1000;
        delay_internal(1000, fac_ms);
    }
    delay_internal(nms, fac_ms);
}

void delay_s(u8 ns)
{
    while(ns > 10)
    {
        ns -= 10;
        delay_internal(10, fac_s);
    }
    delay_internal(ns, fac_s);
}

void system_init()
{
    /* configure systick */
    systick_init();
    /* nvic */
    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_2);
}
