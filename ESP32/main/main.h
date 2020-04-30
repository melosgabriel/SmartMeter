#ifndef __MAIN_H
#define __MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

typedef enum meas_code_t
{
    ADE_MEASURE_FAIL = 0,
    ADE_MEASURE_OK = 1
} meas_code_t;

//* FreeRTOS Queues
extern QueueHandle_t FIFO_Acq_to_Comm;

//* FreeRTOS Tasks
extern TaskHandle_t xTaskCommHandle;
extern TaskHandle_t xTaskAcqHandle;

void task_communication(void * arg);
void task_acquire(void * arg);

#endif /** __MAIN_H */