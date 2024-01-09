#include "debug.h"

#ifdef DEBUG_ON
int SER_PutChar(int ch)
{
    while(!USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC));
    USART_SendData(DEBUG_USART, (uint8_t)ch);
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
