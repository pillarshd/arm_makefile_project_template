#ifndef SYSTICK_H
#define SYSTICK_H

#include "system.h"

#define SYSTICK_SYSCLK  72

void SysTick_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_s(u8 s);

#endif
