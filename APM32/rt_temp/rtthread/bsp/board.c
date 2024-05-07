/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "system.h"
#include "app.h"


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

#if defined (RT_USING_CONSOLE)
#include <stdio.h>
#define LOG_USART                   USART1
#define LOG_CLK                     RCM_APB2_PERIPH_USART1
#define LOG_TX_PIN                  GPIO_PIN_9
#define LOG_RX_PIN                  GPIO_PIN_10
#define LOG_GPIO_PORT               GPIOA
#define LOG_GPIO_CLK                RCM_APB2_PERIPH_GPIOA
void log_init(u32 baud);
#define _RT_KPRINTF_INIT(band)      log_init(band)
#else
#define _RT_KPRINTF_INIT(band)
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    system_init();
    _RT_KPRINTF_INIT(115200);
    led_init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

#if defined (RT_USING_CONSOLE)
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

void rt_hw_console_output(const char *str)
{
    rt_enter_critical();

    while(*str!='\0')
    {
        while (RESET == USART_ReadStatusFlag(LOG_USART, USART_FLAG_TXBE));
        USART_TxData(LOG_USART, (u16)*str++);
    }
    while(RESET == USART_ReadStatusFlag(LOG_USART, USART_FLAG_TXC));
    rt_exit_critical();
}
#endif

