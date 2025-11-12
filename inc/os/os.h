#ifndef INC_OS_H_
#define INC_OS_H_

#include "stdint.h"
#include "stdbool.h"

#include "os_conf.h"
#include "os_critical.h"
#include "task.h"
#include "task_queue.h"
#include "mutex.h"
#include "semaphore.h"

// initialize the OS
void os_init();

// starts the OS scheduler
void os_start();

// delay task for given number of ticks
void os_delay(uint32_t ticks);

// delay task for given number of milliseconds
void os_delay_ms(uint32_t miliseconds);

// yield CPU to another ready task
void os_yield(void);

// yield CPU from an interrupt
void os_yield_from_irq(void);


// called when no task is running (idle)
void os_idle_hook(void);

// called every system tick
void os_tick_hook(void);

#endif /* INC_OS_H_ */
