#include "Systick.h"

static uint8_t fac_us = 0; // 1微秒要的步数


void SysTick_init_internal(u32 sysclk)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
    fac_us = sysclk / 8;
}

void SysTick_init()
{
    SysTick_init_internal(SYSTICK_SYSCLK);
}

void delay_us(u32 nus)
{
    u32 tcnt = 0;
    u32 ticks = nus * fac_us;
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

void delay_ms(u16 nms)
{
    for(int i = 0; i < nms; ++i)
        delay_us(1000);
}

void delay_s(u8 s)
{
    for(int i = 0; i < s; ++i)
        delay_us(1000000);
}
