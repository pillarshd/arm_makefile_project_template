#include "system.h"
#include "app.h"

static struct rt_thread led_thread;
static ALIGN(RT_ALIGN_SIZE) u8 led_thread_stack[256];
static void led_thread_entry(void* parameter)
{
    while (1)
    {
        rt_thread_delay(500);
        led_toggle(LED1);
        rt_kprintf("...led toggle\r\n");
    }
}

int main(void)
{
    rt_enter_critical();

    rt_thread_init(&led_thread, /* 线程控制块 */
                   "led", /* 线程名字 */
                   led_thread_entry, /* 线程入口函数 */
                   RT_NULL, /* 线程入口函数参数 */
                   &led_thread_stack[0], /* 线程栈起始地址 */
                   sizeof(led_thread_stack), /* 线程栈大小 */
                   RT_THREAD_PRIORITY_MAX-1, /* 线程的优先级 */
                   1);/* 线程时间片 */
    rt_thread_startup(&led_thread);
    /* 启动线程，开启调度 */

    rt_exit_critical();
}
