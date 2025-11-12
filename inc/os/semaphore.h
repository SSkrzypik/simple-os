#ifndef INC_OS_SEMAPHORE_H_
#define INC_OS_SEMAPHORE_H_

#include "task_queue.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct {
    uint32_t counter;        // current semaphore value
    uint32_t max;            // maximum semaphore value
    task_queue_t wait_queue; // tasks waiting for the semaphore
} semaphore_t;

// create a binary semaphore
semaphore_t semaphore_create_binary();

// create a counting semaphore with max value
semaphore_t semaphore_create(uint32_t max);

// take (lock) semaphore, optionally block
bool semaphore_take(semaphore_t *semaphore, uint32_t lock_time);

// give (unlock) semaphore, unblock one waiting task
void semaphore_give(semaphore_t *semaphore);

#endif /* INC_OS_SEMAPHORE_H_ */
