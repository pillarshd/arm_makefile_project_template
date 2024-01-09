#ifndef UART_H
#define UART_H

#include "system.h"
#include <stdio.h>

#define UART1_REC_LEN      200
#define UART1_REC_BUFFER_SIZE       1 

#define UART1_PREEMPT_PRIORITY  3
#define UART1_SUB_PRIORITY      3
// extern u8 uart1_RX_buf[UART1_REC_LEN];
// extern u16 uart1_RX_STA;
extern UART_HandleTypeDef uart1_handler;


#define DEBUG_ON
// #undef DEBUG_ON
#ifdef DEBUG_ON

/* Private function prototypes -----------------------------------------------*/
// #if   defined (  ) // #ifdef __CC_ARM
#define PUTCHAR_PROTOTYPE 
// #elif defined ( __GNUC__ ) // __GUNC__ 仿佛也存在，怀疑是环境问题
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
// #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
// #endif 

#define DEBUG_UART         USART1               
#define DEBUG(format, ...)    printf(format, ## __VA_ARGS__)
// #define DEBUG(...)    printf(__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif /* DEBUG_ON */          

void uart1_init(u32 band);
void uart1_send_RX(void);

#endif
