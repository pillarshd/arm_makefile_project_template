#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "system.h"

//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

//断言
#if (LOG_ON != 0)
#define vAssertCalled(char,int) LOG("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)
#endif

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/
/************************************************************************
 *               FreeRTOS基础配置配置选项 
 *********************************************************************/
/* 置1：RTOS使用抢占式调度器；置0：RTOS使用协作式调度器（时间片）
 * 
 * 注：在多任务管理机制上，操作系统可以分为抢占式和协作式两种。
 * 协作式操作系统是任务主动释放CPU后，切换到下一个任务。
 * 任务切换的时机完全取决于正在运行的任务。
 */
#define configUSE_PREEMPTION    1
// 1使能时间片调度(默认式使能的)
#define configUSE_TIME_SLICING  1

/* 某些运行FreeRTOS的硬件有两种方法选择下一个要执行的任务：
 * 通用方法和特定于硬件的方法（以下简称“特殊方法”）。
 * 
 * 通用方法：
 *      1.configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或者硬件不支持这种特殊方法。
 *      2.可以用于所有FreeRTOS支持的硬件
 *      3.完全用C实现，效率略低于特殊方法。
 *      4.不强制要求限制最大可用优先级数目
 * 特殊方法：
 *      1.必须将configUSE_PORT_OPTIMISED_TASK_SELECTION设置为1。
 *      2.依赖一个或多个特定架构的汇编指令（一般是类似计算前导零[CLZ]指令）。
 *      3.比通用方法更高效
 *      4.一般强制限定最大可用优先级数目为32
 * 一般是硬件计算前导零指令，如果所使用的，MCU没有这些硬件指令的话此宏应该设置为0！
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     1  

/* 置1：使能低功耗tickless模式；置0：保持系统节拍（tick）中断一直运行；置2：自己实现
 * 假设开启低功耗的话可能会导致下载出现问题，因为程序在睡眠中,可用以下办法解决
 * 
 * 下载方法：
 *      1.将开发版正常连接好
 *      2.按住复位按键，点击下载瞬间松开复位按键
 *     
 *      1.通过跳线帽将 BOOT 0 接高电平(3.3V)
 *      2.重新上电，下载
 *    
 * 			1.使用FlyMcu擦除一下芯片，然后进行下载
 *			STMISP -> 清除芯片(z)
 */
#define configUSE_TICKLESS_IDLE     0 
// 默认休眠时间，至少大于2个系统节拍
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   2
// 进入低功耗前的自定义操作（如关闭一些外设）
#define configPRE_SLEEP_PROCESSING( x )
// 退出低功耗后的自定义操作（如开启一些外设）
#define configPOST_SLEEP_PROCESSING( x )

/*
 * 写入实际的CPU内核时钟频率，也就是CPU指令执行频率，通常称为Fclk
 * Fclk为供给CPU内核的时钟信号，我们所说的cpu主频为 XX MHz，
 * 就是指的这个时钟信号，相应的，1/Fclk即为cpu时钟周期；
 */
#define configCPU_CLOCK_HZ          (SystemCoreClock)

//RTOS系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度
#define configTICK_RATE_HZ      (( TickType_t )1000)

//可使用的最大优先级 越大越高 0-31 其中0为空闲任务，所以不用
#define configMAX_PRIORITIES        (32)

//空闲任务使用的堆栈大小
#define configMINIMAL_STACK_SIZE        ((unsigned short)128)
  
//任务名字字符串长度
#define configMAX_TASK_NAME_LEN     (16)

 //系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形
#define configUSE_16_BIT_TICKS      0                      

//空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD         1           

//启用队列集
#define configUSE_QUEUE_SETS        0    

//开启任务通知功能，默认开启
#define configUSE_TASK_NOTIFICATIONS    1   
//任务通知数组数，默认为1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1

//使用互斥信号量
#define configUSE_MUTEXES       0    

//使用递归互斥信号量                                            
#define configUSE_RECURSIVE_MUTEXES     0   

//为1时使用计数信号量
#define configUSE_COUNTING_SEMAPHORES       0

/* 设置可以注册的信号量和消息队列个数 */
#define configQUEUE_REGISTRY_SIZE       10                                 
                                                                       
#define configUSE_APPLICATION_TASK_TAG      0

/*****************************************************************
              FreeRTOS与内存申请有关配置选项                                               
*****************************************************************/
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION        1    
//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION     0					
//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE           ((size_t)(8*1024))   

/***************************************************************
             FreeRTOS与钩子函数有关的配置选项                                            
**************************************************************/
/* 置1：使用空闲钩子（Idle Hook类似于回调函数）；置0：忽略空闲钩子
 * 
 * 空闲任务钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationIdleHook(void )，
 * 这个函数在每个空闲任务周期都会被调用
 * 对于已经删除的RTOS任务，空闲任务可以释放分配给它们的堆栈内存。
 * 因此必须保证空闲任务可以被CPU执行
 * 使用空闲钩子函数设置CPU进入省电模式是很常见的
 * 不可以调用会引起空闲任务阻塞的API函数
 */
#define configUSE_IDLE_HOOK         0 

/* 置1：使用时间片钩子（Tick Hook）；置0：忽略时间片钩子
 * 
 * 
 * 时间片钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationTickHook(void )
 * 时间片中断可以周期性的调用
 * 函数必须非常短小，不能大量使用堆栈，
 * 不能调用以”FromISR" 或 "FROM_ISR”结尾的API函数
 */
 /*xTaskIncrementTick函数是在xPortSysTickHandler中断函数中被调用的。因此，vApplicationTickHook()函数执行的时间必须很短才行*/
#define configUSE_TICK_HOOK         0

//使用内存申请失败钩子函数
#define configUSE_MALLOC_FAILED_HOOK        0 

/*
 * 大于0时启用堆栈溢出检测功能，如果使用此功能 
 * 用户必须提供一个栈溢出钩子函数，如果使用的话
 * 此值可以为1或者2，因为有两种栈溢出检测方法 */
#define configCHECK_FOR_STACK_OVERFLOW      0   

/********************************************************************
          FreeRTOS与运行时间和任务状态收集有关的配置选项   
**********************************************************************/
// 会编译prvWriteNameToBuffer()
#define configUSE_STATS_FORMATTING_FUNCTIONS    0  //XXX
 //启用可视化跟踪调试 与上面的1个宏一起不为0会编译vTaskList()--获取任务信息列表
#define configUSE_TRACE_FACILITY        0    
//启用运行时间统计功能 与上面的2个宏一起不为0会编译vTaskGetRunTimeStats()--获取任务运行时间列表
#define configGENERATE_RUN_TIME_STATS   0

#if configGENERATE_RUN_TIME_STATS  == 1
#include "time.h"
extern vu32 FreeRTOSRunTimeTicks;
// 时基定时器初始化
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() \
    {                                            \
        TIM6_init(100-1, 72-1);                   \
        FreeRTOSRunTimeTicks = 0;                \
    }
#define portGET_RUN_TIME_COUNTER_VALUE() FreeRTOSRunTimeTicks
#endif

/********************************************************************
                FreeRTOS与协程有关的配置选项                                                
*********************************************************************/
//启用协程，启用协程以后必须添加文件croutine.c
#define configUSE_CO_ROUTINES           0                 
//协程的有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES     ( 2 )

/***********************************************************************
                FreeRTOS与软件定时器有关的配置选项      
**********************************************************************/
 //启用软件定时器
#define configUSE_TIMERS        0                              
//软件定时器优先级
#define configTIMER_TASK_PRIORITY       (configMAX_PRIORITIES-1)        
//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH        10                               
//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH    (configMINIMAL_STACK_SIZE*2) 

/************************************************************
            FreeRTOS可选函数配置选项                                                     
************************************************************/
// 设置任务优先级                 
#define INCLUDE_vTaskPrioritySet            1 //
// 获取任务优先级
#define INCLUDE_uxTaskPriorityGet           0 //
// 删除任务
#define INCLUDE_vTaskDelete                 1
// 挂起任务
#define INCLUDE_vTaskSuspend                1
// 任务绝对延时
#define INCLUDE_vTaskDelayUntil             0 //
// 任务延时
#define INCLUDE_vTaskDelay                  1
// 获取任务调度器状态
#define INCLUDE_xTaskGetSchedulerState      1   
// 获取当前任务的任务句柄 
#define INCLUDE_xTaskGetCurrentTaskHandle       1
//获取任务堆栈历史剩余最小值
#define INCLUDE_uxTaskGetStackHighWaterMark     1 //
// 获取空闲任务的任务句柄
#define INCLUDE_xTaskGetIdleTaskHandle          0
// 获取任务状态
#define INCLUDE_eTaskGetState                   0 //
//在中断中设置事件标志位
// #define INCLUDE_xEventGroupSetBitFromISR     0
// 将函数的执行挂到定时器服务任务
#define INCLUDE_xTimerPendFunctionCall          0
// 中断任务延时
#define INCLUDE_xTaskAbortDelay                 0
// 通过任务名获取任务句柄
#define INCLUDE_xTaskGetHandle                  0
// 恢复在中断中挂起的任务
#define INCLUDE_xTaskResumeFromISR              1 
#define INCLUDE_vTaskCleanUpResources           0 

/******************************************************************
            FreeRTOS与中断有关的配置选项                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			7     

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
    (lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY     ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
    See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


/****************************************************************
            FreeRTOS与中断服务函数有关的配置选项                         
****************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler

#endif /* FREERTOS_CONFIG_H */
