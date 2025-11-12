#include "os/os.h"
#include "stddef.h"

extern task_t *task_array[MAX_TASKS_COUNT];
extern uint32_t task_current;

mutex_t mutex_create() {
	mutex_t mutex = {};
	mutex.owner = NULL;
	mutex.lock_time = 0;
	return mutex;
}

bool mutex_lock(mutex_t *mutex, uint32_t lock_time) {
    enter_crit();

    if (mutex->owner == NULL) { //taking mutex
        mutex->owner = task_array[task_current];
        mutex->lock_time = lock_time;
        exit_crit();
        return true;
    }
    task_array[task_current]->state = TASK_BLOCKED;
    task_array[task_current]->blocked_reason = BLOCKED_MUTEX;
    task_queue_enqueue(&mutex->wait_queue, task_array[task_current]);

    exit_crit();
    os_yield();
    return false;
}

bool mutex_unlock(mutex_t *mutex) {
    enter_crit();
    if (mutex->owner != task_array[task_current]) {

    	exit_crit();
        return false;
    }

    mutex->owner = NULL;

    task_t *next_task = task_queue_dequeue(&mutex->wait_queue);
    if (next_task != NULL) {
    	if (next_task->delay_ticks == 0) {
    		next_task->state = TASK_RUNNING;
    		next_task->blocked_reason = BLOCKED_NONE;
    	} else {
    		next_task->blocked_reason = BLOCKED_DELAY;
    	}
        mutex->owner = next_task;
    }

    exit_crit();
    return true;
}
