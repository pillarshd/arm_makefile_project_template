#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_s(uint8_t ns);

#endif /* SYSTICK_H */
