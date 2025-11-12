#include "os/task_queue.h"
#include "os/os_critical.h"

#include "stddef.h"

//add to queue
bool task_queue_enqueue(task_queue_t *queue, task_t *task) {
    if (queue == NULL || task == NULL) {
    	return false;
    }

    enter_crit();

    if (queue->count >= MAX_TASKS_COUNT) {
        exit_crit();
        return false;  // queue full
    }

    queue->buffer[queue->tail] = task;
    queue->tail = (queue->tail + 1) % MAX_TASKS_COUNT;
    queue->count++;

    exit_crit();
    return true;
}

// take from queue
task_t *task_queue_dequeue(task_queue_t *queue) {
    if (!queue) return NULL;

    enter_crit();

    if (queue->count == 0) {// queue empty
        exit_crit();
        return NULL;
    }

    task_t *task = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % MAX_TASKS_COUNT;
    queue->count--;

    exit_crit();
    return task;
}
