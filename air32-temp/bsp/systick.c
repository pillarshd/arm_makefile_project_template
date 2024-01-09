#include "systick.h"
#include "air32f10x.h"

static uint32_t fac_us = 0; // 1微秒要的步数
static uint32_t fac_ms = 0;
static uint32_t fac_s = 0;

static void RCC_ClkConfiguration(void)
{
	RCC_DeInit(); //复位RCC寄存器

	RCC_HSEConfig(RCC_HSE_ON); //使能HSE
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); //等待HSE就绪

	RCC_PLLCmd(DISABLE);										 //关闭PLL
	AIR_RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_27, FLASH_Div_2); //配置PLL,8*27=216MHz

	RCC_PLLCmd(ENABLE); //使能PLL
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL就绪

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //选择PLL作为系统时钟

	RCC_HCLKConfig(RCC_SYSCLK_Div1); //配置AHB时钟
	RCC_PCLK1Config(RCC_HCLK_Div2);	 //配置APB1时钟
	RCC_PCLK2Config(RCC_HCLK_Div1);	 //配置APB2时钟

	RCC_LSICmd(ENABLE); //使能内部低速时钟
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);				//等待LSI就绪
	RCC_HSICmd(ENABLE); //使能内部高速时钟
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //等待HSI就绪
}

void systick_init()
{
    RCC_ClkConfiguration();
    RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks); //获取时钟频率
	SystemCoreClock = clocks.HCLK_Frequency;				
    SysTick_Config(SystemCoreClock / 1000U);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
    fac_us = SystemCoreClock / 1000000U / 8U;
    fac_ms = SystemCoreClock / 1000U / 8U;
    fac_s = SystemCoreClock / 8U;
}

static void delay_internal(uint32_t n, uint32_t fac)
{
    uint32_t tcnt = 0;
    uint32_t ticks = n * fac;
    uint32_t tnow = 0;
    uint32_t told = SysTick->VAL;
    uint32_t reload = SysTick->LOAD;

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

void delay_us(uint32_t nus)
{
    delay_internal(nus, fac_us);
}


void delay_ms(uint16_t nms)
{
    delay_internal(nms, fac_ms);
}

void delay_s(uint8_t ns)
{
    delay_internal(ns, fac_s);
}
