#ifndef INC_OS_OS_CONF_H_
#define INC_OS_OS_CONF_H_

#define OS_MAX_DELAY 0xFFFFFFFFU       // max delay value (in ticks)
#define MAX_STACK_SIZE 128             // max stack size in words per task

#define MAX_TASK_NAME_LENGTH 32        // max length of task name
#define MAX_TASKS_COUNT 32             // max number of tasks
#define MAX_TASK_PRIORITY 16           // max task priority

#define IDLE_TASK_STACK_SIZE 128       // stack size for idle task

#define PENDSV_IRQ_PRIORITY 0xFF       // PendSV interrupt priority
#define SYSTICK_IRQ_PRIORITY 15        // SysTick interrupt priority

#define OS_TICK_PER_SECOND 1000        // system ticks per second
#define CPU_FREQUENCY 64000000         // CPU clock frequency in Hz

#define IDLE_TASK_NAME "idle_task"     // name of idle task

#endif /* INC_OS_OS_CONF_H_ */
