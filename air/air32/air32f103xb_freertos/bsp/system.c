#include "system.h"

#if (LOG_ON != 0)

void log_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(LOG_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(LOG_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LOG_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LOG_GPIO_PORT, &GPIO_InitStructure);
    // GPIO_InitStructure.GPIO_Pin = LOG_RX_PIN;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    // GPIO_Init(LOG_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;// | USART_Mode_Rx;

    USART_Init(LOG_USART, &USART_InitStructure);
    USART_Cmd(LOG_USART, ENABLE);
}
int SER_PutChar(int ch)
{
    while(!USART_GetFlagStatus(LOG_USART, USART_FLAG_TC));
    USART_SendData(LOG_USART, (uint8_t)ch);
    return ch;
}
int _write(int fd, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        SER_PutChar(*ptr++);
    }
    return len;
}

#endif

///////////////////////////////// SysTick ////////////////////////////////////
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

	// RCC_LSICmd(ENABLE); //使能内部低速时钟
	// while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);				//等待LSI就绪
	// RCC_HSICmd(ENABLE); //使能内部高速时钟
	// while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //等待HSI就绪
	RCC_LSICmd(DISABLE); //使能内部低速时钟
	// while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);				//等待LSI就绪
	RCC_HSICmd(DISABLE); //使能内部高速时钟
}

static void systick_init()
{
    RCC_ClkConfiguration();
    RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks); //获取时钟频率
	SystemCoreClock = clocks.HCLK_Frequency;				
    fac_us = SystemCoreClock / 1000000U / 8U;
    fac_ms = SystemCoreClock / 1000U / 8U;
    fac_s = SystemCoreClock / 8U;
    SysTick_Config(fac_ms);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
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

void system_init()
{
    /* nvic */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    /* configure systick */
    systick_init();
}

