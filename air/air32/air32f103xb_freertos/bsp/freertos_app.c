#include "freertos_app.h"


#define LED_TASK_PRIO		1       // 任务优先级
#define LED_STK_SIZE 		50      // 任务堆栈大小 
TaskHandle_t led_task_handler;       // 任务句柄
static void led_task(void *pvParameters);  // 任务函数

#define LOG_TASK_PRIO       1
#define LOG_TASK_STACK_SIZE     400
TaskHandle_t LOG_task_handler;
static void LOG_task(void *pvParameters);

void freertos_app()
{
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led_task,     
                (const char*    )"led1task",   
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&led_task_handler);

    xTaskCreate((TaskFunction_t )LOG_task,
                (const char *   )"LOG_task",
                (u16            )LOG_TASK_STACK_SIZE,
                (void *         )NULL,
                (UBaseType_t    )LOG_TASK_PRIO,
                (TaskHandle_t * )&LOG_task_handler);
    vTaskStartScheduler();          //开启任务调度
    err(); 
} 

//LED1任务函数
static void led_task(void *pvParameters)
{
    while (1)
    {
        LED_TURN;
        vTaskDelay(500);
    }
}

static void LOG_task(void *pvParameters)
{
    while (1)
    {
        // char buf[300];
        // vTaskList(buf);
        // FLOG("%s\r\n",buf);
        // vTaskGetRunTimeStats(buf);
        // FLOG("%s\r\n",buf);
        FLOG("LED empty stack size : %ld\r\n",uxTaskGetStackHighWaterMark(led_task_handler));
        FLOG("LOG empty stack size : %ld\r\n",uxTaskGetStackHighWaterMark(LOG_task_handler));
        vTaskDelay(1000);
    }
}
