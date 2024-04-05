#ifndef _SYSTEM_H
#define _SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_ON  1

/* LL Includes */
#include "air001xx_ll_adc.h"
#include "air001xx_ll_crc.h"
#include "air001xx_ll_exti.h"
#include "air001xx_ll_led.h"
#include "air001xx_ll_rtc.h"
#include "air001xx_ll_usart.h"
#include "air001xx_ll_bus.h"
#include "air001xx_ll_crs.h"
#include "air001xx_ll_gpio.h"
#include "air001xx_ll_lptim.h"
#include "air001xx_ll_spi.h"
// #include "air001xx_ll_usb.h" //HACK:需要HAL库
#include "air001xx_ll_comp.h"
#include "air001xx_ll_dac.h"
#include "air001xx_ll_i2c.h"
#include "air001xx_ll_pwr.h"
#include "air001xx_ll_system.h"
#include "air001xx_ll_utils.h"
#include "air001xx_ll_cortex.h"
#include "air001xx_ll_dma.h"
#include "air001xx_ll_iwdg.h"
#include "air001xx_ll_rcc.h"
#include "air001xx_ll_tim.h"
#include "air001xx_ll_wwdg.h"

#if defined(USE_FULL_ASSERT)
#include "air001_assert.h"
#endif /* USE_FULL_ASSERT */

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
// #define LOG_RX_GPIO_PORT        GPIOA
// #define LOG_RX_GPIO_CLK         LL_IOP_GRP1_PERIPH_GPIOA
// #define LOG_RX_PIN              LL_GPIO_PIN_3
// #define LOG_RX_AF               LL_GPIO_AF4_USART2

#define LOG_TX_GPIO_PORT        GPIOA
#define LOG_TX_GPIO_CLK         LL_IOP_GRP1_PERIPH_GPIOA 
#define LOG_TX_PIN              LL_GPIO_PIN_2
#define LOG_TX_AF               LL_GPIO_AF4_USART2

#define LOG_CLK     LL_APB1_GRP1_PERIPH_USART2
#define LOG_USART   USART2    
void log_init(u32 baud);
#define LOG_INIT(baud)          log_init(baud)
#define LOG(format, ...)        printf(format, ## __VA_ARGS__) 

#else

#define LOG_INIT(band) 
#define LOG(format, ...) 

#endif

#define Error_Handler() while(1)
void set_system_clock(void);

static inline void delay_ms(u32 delay)
{
    LL_Init1msTick(SystemCoreClock);
    LL_mDelay(delay);
}
static inline void delay_us(u32 delay)
{
    if((delay*(SystemCoreClock/1000000)) > 0xFFFFFF)
        Error_Handler();
    LL_uDelay(delay);
}

#ifdef __cplusplus
}
#endif

#endif
