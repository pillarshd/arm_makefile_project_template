#include "system.h"
#include "freertos_app.h"

int main(void)
{
    system_init();
    LOG_INIT(115200);
    led1_init();    
    LOG("FreeRTOS Start!!!\r\n");
    freertos_app();
}
