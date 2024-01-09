#include "led.h"

void led_init(void)
{
    LED1_PORT_RCC_ENABLE();           	//开启GPIOB时钟
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=LED1_PIN; 				//PB5
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_InitStructure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(LED1_PORT,&GPIO_InitStructure);
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);//PB5输出高
#if LED2
    LED2_PORT_RCC_ENABLE();           	//开启GPIOE时钟
	GPIO_InitStructure.Pin=LED2_PIN; 				//PE5
	HAL_GPIO_Init(LED2_PORT,&GPIO_InitStructure);
	HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET);//PE5输出高
#endif
}

