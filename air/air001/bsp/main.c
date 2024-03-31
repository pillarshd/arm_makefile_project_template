#include "system.h"
#include "led.h"

int main(void)
{
    /* 初始化所有外设，Flash接口，SysTick */
    HAL_Init();
    /* system_init */
    set_system_clock();
    LOG_INIT(115200);
    /* init */
    led_init();

    LOG("Loop Start!!!\r\n");
    while (1)
    {
        HAL_Delay(250);
        LED_TOGGLE();
    }
}
