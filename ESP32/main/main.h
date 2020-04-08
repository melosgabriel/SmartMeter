#ifndef __MAIN_H
#define __MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

//* Declaring freeRTOS tasks here
void task_communication(void * arg);
void task_acquire(void * arg);

#endif /** __MAIN_H */