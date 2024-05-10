#include "i2c.h"

void i2c_init()
{
    LOG("i2c init\r\n");
    GPIO_Config_T gpioConfigStruct;
    I2C_SCL_CLK_EN();
    gpioConfigStruct.mode = GPIO_MODE_AF_OD;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    gpioConfigStruct.pin = I2C_SCL_PIN;
    GPIO_Config(I2C_SCL_PORT, &gpioConfigStruct);
    I2C_SDA_CLK_EN();
    gpioConfigStruct.pin = I2C_SDA_PIN;
    GPIO_Config(I2C_SDA_PORT, &gpioConfigStruct);

    /* Config I2C1 */
    RCM_EnableAPB1PeriphClock(I2C_CLK);
    I2C_Reset(I2C);
    I2C_Config_T i2cConfigStruct;
    i2cConfigStruct.mode = I2C_MODE_I2C;
    i2cConfigStruct.dutyCycle = I2C_DUTYCYCLE_2;
    i2cConfigStruct.ackAddress = I2C_ACK_ADDRESS_7BIT;
    i2cConfigStruct.ownAddress1 = 0XA0;
    i2cConfigStruct.ack = I2C_ACK_ENABLE;
    i2cConfigStruct.clockSpeed = 100000;
    I2C_Config(I2C, &i2cConfigStruct);
    /* Enable I2Cx */
    I2C_Enable(I2C);
    set_address_to_send(QMI8658_ADDRESS);
}

#if defined (I2C_WAIT_FOREVER)
#define I2C_WAIT_EVENT(event) while (I2C_ReadEventStatus(I2C, event) != SUCCESS);
#else
#define I2C_WAIT_EVENT(event)                              \
    do                                                     \
    {                                                      \
        u32 Timeout = 10000;                               \
        while (I2C_ReadEventStatus(I2C, event) != SUCCESS) \
        {                                                  \
            Timeout--;                                     \
            if (Timeout == 0)                              \
                return;                                    \
        }                                                  \
    } while (0)
#endif

void i2c_write(u8 address, u8 *buf, u32 size)
{
    /* 发送开始位 */
    I2C_EnableGenerateStart(I2C);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT); /* 检测EV5 */
    /* 发送从机地址 */
    I2C_Tx7BitAddress(I2C, address, I2C_DIRECTION_TX);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); /* 检测EV6（发送模式）*/
    /* 发送数据 */
    while(size--)
    {
        I2C_TxData(I2C, *buf++);
        I2C_WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING); /* 检测EV8 */
    }
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED); /* 结束发送，检测EV8_2 */
    /* 发送停止位 */
    I2C_EnableGenerateStop(I2C);
}

void i2c_read(u8 address, u8 reg, u8 *buf, u32 size)
{
    /* 发送开始位 */
    I2C_EnableGenerateStart(I2C);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT); /* 检测EV5 */
    /* 发送从机地址 */
    I2C_Tx7BitAddress(I2C, address, I2C_DIRECTION_TX);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); /* 检测EV6（发送模式）*/
    /* 发送从机寄存器地址 */
    I2C_TxData(I2C, reg);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED); /* 检测EV8 */
    /* 再次发送开始位 */
    I2C_EnableGenerateStart(I2C);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT); /* 检测EV5 */
    /* 再次发送从机地址 */
    I2C_Tx7BitAddress(I2C, address, I2C_DIRECTION_RX);
    I2C_WAIT_EVENT(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); /* 检测EV6（发送模式）*/
    while(size)
    {
        /* 结束-no ACK */
        if(size == 1)
        {
            I2C_DisableAcknowledge(I2C); /* 主机给从机应答位 */
        }
        size--;
        /* 获取数据 */
        I2C_WAIT_EVENT(I2C_EVENT_MASTER_BYTE_RECEIVED); /* 检测EV7 */
        *buf++ = I2C_RxData(I2C);
    }
    /* 发送停止位 */
    I2C_EnableGenerateStop(I2C);
    I2C_EnableAcknowledge(I2C); /* 恢复ACK */
}

static u8 address_to_send;

void set_address_to_send(u8 address)
{
    address_to_send = address;
}


void i2c_write_reg(u8 reg, u8 data)
{
    u8 buf[2] = { reg, data };
    i2c_write(address_to_send, buf, sizeof(buf));
}

u8 i2c_read_reg(u8 reg)
{
    u8 data;
    i2c_read(address_to_send, reg, &data, 1);
    return data;
}
