#ifndef INC_OS_TASK_H_
#define INC_OS_TASK_H_

#include "os_conf.h"
#include "stdint.h"

typedef uint8_t task_priority_t;

typedef enum {
    BLOCKED_NONE,
    BLOCKED_DELAY,
    BLOCKED_MUTEX,
    BLOCKED_SEMAPHORE,
    BLOCKED_IO,
    BLOCKED_OTHER
} blocked_reason_t;

typedef enum {
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED,
    TASK_SUSPENDED,
    TASK_TERMINATED
} task_state_t;

typedef enum {
    TASK_ALLOC_STATIC,
    TASK_ALLOC_DYNAMIC
} task_alloc_type_t;

typedef struct {
	uint32_t *stack_base;           // base of task stack
	uint32_t psp;                   // current process stack pointer
	void (*handler)(void);          // task function

	volatile blocked_reason_t blocked_reason; // why task is blocked
	volatile task_state_t state;             // current task state

	volatile uint32_t delay_ticks;  // remaining ticks for delay

	uint32_t id;                    // unique task ID
	task_priority_t priority;       // task priority
	task_alloc_type_t alloc_type;   // dynamic or static allocation
	char name[MAX_TASK_NAME_LENGTH]; // task name

} task_t;

// create a task using malloc()
task_t *task_create(void (*handler)(void), const char *task_name, task_priority_t priority, uint32_t stack_size);

// create a task with user-provided stack and buffer
void task_create_static(void (*handler)(void), const char *task_name, task_priority_t priority, uint32_t stack_size, uint32_t *stack_buffer, task_t *task_buffer);

void task_delete(task_t *task);

void task_suspend(task_t *task);

void task_resume(task_t *task);

task_t *task_get_current(void);

#endif /* INC_OS_TASK_H_ */
