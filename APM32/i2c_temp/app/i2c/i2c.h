#ifndef _I2C_H
#define _I2C_H

#include "system.h"

#define QMI8658_ADDRESS		0x6A

#define I2C_WAIT_FOREVER
#define I2C         I2C1
#define I2C_CLK     RCM_APB1_PERIPH_I2C1

#define I2C_SCL_CLK_EN()    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO)
#define I2C_SCL_PORT        GPIOB
#define I2C_SCL_PIN         GPIO_PIN_6

#define I2C_SDA_CLK_EN()    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO)
#define I2C_SDA_PORT        GPIOB
#define I2C_SDA_PIN         GPIO_PIN_7

void i2c_init(void);
void i2c_write(u8 address, u8 *buf, u32 size);
void i2c_read(u8 adresss, u8 reg, u8 *buf, u32 size);

void set_address_to_send(u8 address);
void i2c_write_reg(u8 reg, u8 Data);
u8 i2c_read_reg(u8 reg);

#endif
