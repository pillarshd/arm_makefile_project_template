#include "system.h" 

static u8 fac_us = 0; // 1微秒要的步数
static u16 fac_ms = 0;
static u32 fac_s = 0;

// 定义了OS优先级，说明有OS
#ifdef RT_THREAD_PRIORITY_MAX 
#define _ENTER_CRITICAL()   rt_enter_critical()
#define _EXIT_CRITICAL()    rt_exit_critical()
#else
#define _ENTER_CRITICAL()
#define _EXIT_CRITICAL()
#endif

static void systick_init()
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND / 8);
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
    _ENTER_CRITICAL();
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
    _EXIT_CRITICAL();
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
    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_4);
}
