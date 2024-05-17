#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "air001xx_hal.h"

#define LOG_ON  1

/* 定义一些常用的数据类型短关键字 */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;
typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;
typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;
typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;
typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;
typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;

#if (LOG_ON != 0)

#include <stdio.h>
#define LOG_RX_GPIO_PORT            GPIOA
#define LOG_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define LOG_RX_PIN                  GPIO_PIN_3
#define LOG_RX_AF                   GPIO_AF4_USART2

#define LOG_TX_GPIO_PORT            GPIOA
#define LOG_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define LOG_TX_PIN                  GPIO_PIN_2
#define LOG_TX_AF                   GPIO_AF4_USART2

#define LOG_CLK_ENABLE()    __HAL_RCC_USART2_CLK_ENABLE()
#define LOG_USART           USART2
void log_init(u32 baud);
extern UART_HandleTypeDef log_uart_handle;
#define LOG_INIT(baud)          log_init(baud)
#define LOG(format, ...)        printf(format, ## __VA_ARGS__)

#else

#define LOG_INIT(band)
#define LOG(format, ...)

#endif

#define Error_Handler() while(1)
void set_system_clock(void);

#endif
