#ifndef DEBUG_H
#define DEBUG_H

#include "system.h"
#include "usart.h"

#define DEBUG_ON
// #undef DEBUG_ON
#ifdef DEBUG_ON
#define DEBUG_USART           USART1
#define DEBUG_INIT(band)      usart1_init(band)
#define DEBUG(format, ...)    printf(format, ## __VA_ARGS__)
// #define DEBUG(...)            printf(__VA_ARGS__)
#else
#define DEBUG_INIT(band)
#define DEBUG(format, ...)
#endif /* DEBUG_ON */       

#endif
