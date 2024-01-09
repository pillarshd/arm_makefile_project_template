#include "main.h"

int main(void)
{
    /* STM32F103xG HAL library initialization:
        - Configure the Flash prefetch
        - Systick timer is configured by default as source of time base, but user 
            can eventually implement his proper time base source (a general purpose 
            timer for example or other time source), keeping in mind that Time base 
            duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
            handled in milliseconds basis.
        - Set NVIC Group Priority to 4
        - Low Level Initialization */
    HAL_Init();
    /* Configure the system clock to 72 MHz */
    SystemClock_Init(RCC_PLL_MUL9);
    /* Systick init HAL_Delay失效 1s约等于HAL_Delay(120) */
    SysTick_init();
    /* Other init*/
    uart1_init(115200);
    led_init();
    /* Add your application code here */
    /* Infinite loop */
    float i = 3.14;
    DEBUG("aaa%d  %f\r\n",(int)(i*100), i);
    DEBUG("Loop start!\r\n");
    while (1)
    {
        delay_ms(500);
        LED1_OUT = !LED1_OUT;
        LED2_OUT = !LED2_OUT;
    }
}
