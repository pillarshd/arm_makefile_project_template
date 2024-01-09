#include "debug.h"

UART_HandleTypeDef uart1_handler;
u8 uart1_RX_buf[UART1_REC_LEN]; // 接收缓存
u8 uart1_RX_temp_buf[UART1_REC_BUFFER_SIZE]; // 临时接收缓冲
/*****************************************************************************
 * @brief usart1_RX_STA含义
 * bit15    接收完成标志
 * bit14    接收到 0x0d
 * bit13~0  接收到的有效
 ****************************************************************************/
u16 uart1_RX_STA = 0; 


/* Init the low level hardware : GPIO, CLOCK */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_Init_structure;

    if(huart->Instance == USART1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        GPIO_Init_structure.Pin = GPIO_PIN_10;
        GPIO_Init_structure.Mode = GPIO_MODE_AF_INPUT;
        GPIO_Init_structure.Pull = GPIO_PULLUP; // 防止干扰
        GPIO_Init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA,&GPIO_Init_structure);

        GPIO_Init_structure.Pin = GPIO_PIN_9;
        GPIO_Init_structure.Mode = GPIO_MODE_AF_PP;
        HAL_GPIO_Init(GPIOA,&GPIO_Init_structure);      
    }
}

void uart1_init(u32 band) // 波特率
{
    uart1_handler.Instance = USART1;
    uart1_handler.Init.BaudRate = band;
    uart1_handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长
    uart1_handler.Init.StopBits = UART_STOPBITS_1; // 停止位
    uart1_handler.Init.Parity = UART_PARITY_NONE; // 奇偶校验
    uart1_handler.Init.Mode = UART_MODE_TX_RX; // 奇偶校验
    uart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //硬件流设置
#if defined(USART_CR1_OVER8)
    uart1_handler.Init.OverSampling =  UART_OVERSAMPLING_8; // 过采样设置
#endif
    HAL_UART_Init(&uart1_handler);
    /* 开启中断中的UART_IT_RXNE接收完成中断 */
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn, UART1_PREEMPT_PRIORITY, UART1_SUB_PRIORITY);
    //不使用 HAL 库中的 HAL_UART_Receive_IT 函数，使能接收中断函数
    // __HAL_UART_ENABLE_IT(&uart1_handler,UART_IT_RXNE);
    /* 该函数会开启接收中断：标志位 UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&uart1_handler, (u8 *)uart1_RX_temp_buf, UART1_REC_BUFFER_SIZE);
}

/*****************************************************************************
 * @brief 思路
 * 当接收到从电脑发过来的数据，把接收到的数据保存在 uart1_RX_BUF 中，
 * 同时在接收状态寄存器（uart1_RX_STA）中计数接收到的有效数据个数，当收
 * 到回车（回车的表示由 2 个字节组成：0X0D 和 0X0A）的第一个字节 0X0D 时，
 * 计数器将不再增加，等待 0X0A 的到来，而如果 0X0A 没有来到，则认为这次接
 * 收失败，重新开始下一次接收。如果顺利接收到 0X0A，则标记 uart1_RX_STA 的
 * 第 15 位，这样完成一次接收，并等待该位被其他程序清除，从而开始下一次的
 * 接收，而如果迟迟没有收到 0X0D，那么在接收数据超过 uart1_REC_LEN 的时
 * 候，则会丢弃前面的数据，重新接收
 ****************************************************************************/
// UART1_REC_BUFFER_SIZE为1时才会每次调用
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)//如果是串口1
    {
        if((uart1_RX_STA&0x8000) == 0) // 接收未完成
		{
			if(uart1_RX_STA & 0x4000) // 接收到过0x0d \r
			{
				if(uart1_RX_temp_buf[0] != 0x0a) // 不是0x0a \n，错误
                    uart1_RX_STA = 0;
				else                             // 接收完成了
                    uart1_RX_STA |= 0x8000; 
			}
			else 
			{	
				if(uart1_RX_temp_buf[0] == 0x0d) // 接收到0x0d
                {
                    uart1_RX_STA |= 0x4000;
                }
				else                             // 正常信息
				{
					uart1_RX_buf[uart1_RX_STA&0X3FFF] = uart1_RX_temp_buf[0];
					uart1_RX_STA++;
					if(uart1_RX_STA > (UART1_REC_LEN-1)) // 缓存溢出
                        uart1_RX_STA = 0;	  
				}		 
			}
        }
    }   
    /* 重新使能中断 */
    u32 timeout = 0;
    // while (HAL_UART_GetState(&uart1_handler) != HAL_UART_STATE_READY)
    // {
    //     if(++timeout > HAL_MAX_DELAY) 
    //         break;	
    // }
    // timeout = 0;
    while(HAL_UART_Receive_IT(&uart1_handler, (u8 *)uart1_RX_temp_buf, 
                              UART1_REC_BUFFER_SIZE) != HAL_OK)
    {
        if(++timeout > HAL_MAX_DELAY) 
            break;	
    }
}


void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&uart1_handler);
    uart1_send_RX();
}

// 直接把中断控制逻辑写在服务函数中，效率更高
// void USART1_IRQHandler(void)
// {
//     u8 data = 0;
//     //接收中断(接收到的数据必须是 0x0d 0x0a 结尾)
//     if((__HAL_UART_GET_FLAG(&uart1_handler,UART_FLAG_RXNE) != RESET)) 
//     {
//         HAL_UART_Receive(&uart1_handler,&data,1,1000);
//         if((uart1_RX_STA&0x8000)==0)//接收未完成
//         {
//             if(uart1_RX_STA&0x4000)//接收到了 0x0d
//             {
//                 if(data!=0x0a)
//                     uart1_RX_STA=0;//接收错误,重新开始
//                 else 
//                     uart1_RX_STA|=0x8000; //接收完成了
//             }
//             else //还没收到 0X0D
//             {
//                 if(data==0x0d)
//                 {
//                     uart1_RX_STA|=0x4000;
//                 }
//                 else 
//                 {
//                     uart1_RX_buf[uart1_RX_STA&0X3FFF]=data;
//                     uart1_RX_STA++;
//                     if(uart1_RX_STA>(UART1_REC_LEN-1))
//                         uart1_RX_STA=0;//接收数据错误,重新开
//                 }
//             }
//         }
//     }
//     HAL_UART_IRQHandler(&uart1_handler);
// }

void uart1_send_RX(void)
{
    if(uart1_RX_STA & 0x8000)
    {
        // 发送
        HAL_UART_Transmit(&uart1_handler, uart1_RX_buf, 
                          uart1_RX_STA&0x3FFF, 1000); 
        // 发送完成
        while(__HAL_UART_GET_FLAG(&uart1_handler,UART_FLAG_TC)!=SET);
        uart1_RX_STA = 0;
    }
}

/*****************************************************************************
 * @brief printf重定向
 ****************************************************************************/
#ifdef  DEBUG_ON
#ifdef __GNUC__
int _write(int fd, char *ptr, int len)  
{
    HAL_UART_Transmit(&uart1_handler, (uint8_t *)ptr, len, 0xFFFF);
    return len;
}
#elif __clang__
int fputc(int ch, FILE *p)
{
    while((DEBUG_UART->SR&0X40)==0);//之前的发送完毕
    DEBUG_UART->DR = ch;
    return ch;
}
#endif
#endif
