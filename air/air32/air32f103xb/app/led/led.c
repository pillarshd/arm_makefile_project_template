#include "led.h"

/* GPIO PORT */
void led1_init(void)
{
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE); //使能GPIOB时钟
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //输出模式
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure); //初始化GPIOB.2,10,11
    LED1_OUT = 1;
}
