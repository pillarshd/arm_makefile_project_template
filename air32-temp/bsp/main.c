#include "system.h"
#include "debug.h"
#include "led.h"

int main(void)
{
    system_init();
    DEBUG_INIT(115200);
    led1_init();    
    DEBUG("loop %d  %f start\r\n",1,3.14f);
    while (1)
    {
        LED_TURN;
        delay_ms(500);
    }
}
