#include "system.h"
#include "app.h"

int main(void)
{
    system_init();
    LOG_INIT(115200);
    led1_init();    
    LOG("loop start\r\n");
    while (1)
    {
        LED_TURN;
        delay_ms(500);
    }
}
