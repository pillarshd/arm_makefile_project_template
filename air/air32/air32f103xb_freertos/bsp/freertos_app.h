#ifndef FREERTOS_APP_H
#define FREERTOS_APP_H

#include "system.h"
#include "FreeRTOS.h"
#include "task.h"

#include "app.h"

#define FLOG(format, ...)           \
    do{                               \
        taskENTER_CRITICAL();         \
        LOG(format, ##__VA_ARGS__);   \
        taskEXIT_CRITICAL();          \
    } while (0)

void freertos_app(void);

#endif
