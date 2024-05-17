#ifndef SYSTEM_H
#define SYSTEM_H

#include "apm32f10x.h"
#include "apm32f10x_adc.h"
#include "apm32f10x_dbgmcu.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_qspi.h"
#include "apm32f10x_smc.h"
#include "apm32f10x_bakpr.h"
#include "apm32f10x_dma.h"
#include "apm32f10x_i2c.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_spi.h"
#include "apm32f10x_can.h"
#include "apm32f10x_dmc.h"
#include "apm32f10x_iwdt.h"
#include "apm32f10x_rtc.h"
#include "apm32f10x_tmr.h"
#include "apm32f10x_crc.h"
#include "apm32f10x_eint.h"
#include "apm32f10x_misc.h"
#include "apm32f10x_sci2c.h"
#include "apm32f10x_usart.h"
#include "apm32f10x_dac.h"
#include "apm32f10x_fmc.h"
#include "apm32f10x_pmu.h"
#include "apm32f10x_sdio.h"
#include "apm32f10x_wwdt.h"
#include <stdint.h>

#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * @brief 位带区转位带别名区地址
 *        bit_word_addr =bit_band_base + (byte_offset×32) + (bit_number×4)
 ****************************************************************************/
#define _BITBAND(addr, bitnum)       ((addr & 0xF0000000) + 0x02000000) + \
                                    ((addr & 0x000FFFFF) << 5) + (bitnum << 2)
#define _MEM_ADDR(addr)              (*((volatile uint32_t *)(addr)))
#define _BIT_ADDR(addr, bitnum)      _MEM_ADDR(BITBAND(addr, bitnum))

/* IO口输出地址映射 */
#define GPIOA_ODR_ADDR    (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_ADDR    (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_ADDR    (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_ADDR    (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_ADDR    (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_ADDR    (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_ADDR    (GPIOG_BASE + 12) // 0x40011E0C
/* IO口输入地址映射 */
#define GPIOA_IDR_ADDR    (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_ADDR    (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_ADDR    (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_ADDR    (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_ADDR    (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_ADDR    (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_ADDR    (GPIOG_BASE + 8) // 0x40011E08

/* 输出 */
#define _PAOUT(n)   _BIT_ADDR(GPIOA_ODR_ADDR, n)
#define _PBOUT(n)   _BIT_ADDR(GPIOB_ODR_ADDR, n)
#define _PCOUT(n)   _BIT_ADDR(GPIOC_ODR_ADDR, n)
#define _PDOUT(n)   _BIT_ADDR(GPIOD_ODR_ADDR, n)
#define _PEOUT(n)   _BIT_ADDR(GPIOE_ODR_ADDR, n)
#define _PFOUT(n)   _BIT_ADDR(GPIOF_ODR_ADDR, n)
#define _PGOUT(n)   _BIT_ADDR(GPIOG_ODR_ADDR, n)
/* 输入 */
#define _PAIN(n)    _BIT_ADDR(GPIOA_IDR_ADDR, n)
#define _PBIN(n)    _BIT_ADDR(GPIOB_IDR_ADDR, n)
#define _PCIN(n)    _BIT_ADDR(GPIOC_IDR_ADDR, n)
#define _PDIN(n)    _BIT_ADDR(GPIOD_IDR_ADDR, n)
#define _PEIN(n)    _BIT_ADDR(GPIOE_IDR_ADDR, n)
#define _PFIN(n)    _BIT_ADDR(GPIOF_IDR_ADDR, n)
#define _PGIN(n)    _BIT_ADDR(GPIOG_IDR_ADDR, n)

/*****************************************************************************
 * @brief LOG
 ****************************************************************************/


void system_init(void);

void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_s(u8 ns);

#ifdef __cplusplus
}
#endif

#endif
