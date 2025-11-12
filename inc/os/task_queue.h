#ifndef INC_OS_TASK_QUEUE_H_
#define INC_OS_TASK_QUEUE_H_

#include "task.h"
#include "os_conf.h"
#include "stdbool.h"

typedef struct {
    task_t *buffer[MAX_TASKS_COUNT]; // tasks in queue
    uint32_t head;   // index of first task
    uint32_t tail;   // index of next free slot
    uint32_t count;  // number of tasks in queue
} task_queue_t;

// initialize queue
static inline void task_queue_init(task_queue_t *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
}

// get number of tasks in queue
static inline uint32_t task_queue_count(task_queue_t *queue) {
    return queue->count;
}

// add task to queue
bool task_queue_enqueue(task_queue_t *queue, task_t *task);

// remove task from queue
task_t *task_queue_dequeue(task_queue_t *queue);

#endif /* INC_OS_TASK_QUEUE_H_ */
