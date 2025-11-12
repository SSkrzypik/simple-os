#include "os/os.h"
#include "stddef.h"

extern task_t *task_array[MAX_TASKS_COUNT];
extern uint32_t task_current;

semaphore_t semaphore_create_binary() {
	return semaphore_create(1);
}

semaphore_t semaphore_create(uint32_t max) {
	semaphore_t semaphore = {};
	semaphore.max = max;
	semaphore.counter = 0;
	return semaphore;
}

bool semaphore_take(semaphore_t *semaphore, uint32_t lock_time) {
	enter_crit();

    if (semaphore->counter > 0) {
        semaphore->counter--; //taking semaphore
        exit_crit();
        return true;
    }

    task_array[task_current]->state = TASK_BLOCKED;
    task_array[task_current]->blocked_reason = BLOCKED_SEMAPHORE;
    task_queue_enqueue(&semaphore->wait_queue, task_array[task_current]);

    exit_crit();
    os_yield();
    return false;
}

void semaphore_give(semaphore_t *semaphore) {
    enter_crit();

    task_t *next_task = task_queue_dequeue(&semaphore->wait_queue);
    if (next_task != NULL) {
        next_task->state = TASK_RUNNING;
        next_task->blocked_reason = BLOCKED_NONE;
    } else {
        if (semaphore->counter < semaphore->max) {
            semaphore->counter++;
        }
    }

    exit_crit();
}
