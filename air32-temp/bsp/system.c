#include "system.h"

void system_init()
{
    /* configure systick */
    systick_init();
    /* nvic */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}
