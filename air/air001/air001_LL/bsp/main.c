#include "system.h"
#include "app.h"

int main(void)
{
    set_system_clock();
    LOG_INIT(115200);
    LOG("Loop Star!!!\r\n");
    while (1)
    {
        delay_ms(500);
        LED_TOGGLE();
    }
}
