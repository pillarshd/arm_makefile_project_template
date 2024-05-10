#include "system.h"
#include "app.h"

int main(void)
{
    system_init();
    LOG_INIT(115200);
    led_init();  
    i2c_init();
    LOG("Loop Start!!!\r\n");
    while (1)
    {
        delay_ms(500);
        led_toggle(LED1);
        LOG("...led toggle\r\n");
        LOG("0x%x\r\n",i2c_read_reg(0x0));
    }
}
