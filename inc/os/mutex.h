#ifndef INC_MUTEX_H_
#define INC_MUTEX_H_

#include "task_queue.h"

typedef struct {
    task_t *owner;           // Task currently owning the mutex
    uint32_t lock_time;      // Time the mutex has been locked
    task_queue_t wait_queue; // Queue of tasks waiting for the mutex
} mutex_t;


// create a new mutex
mutex_t mutex_create(void);

// lock a mutex, optionally wait with timeout
bool mutex_lock(mutex_t *mutex, uint32_t lock_time);

// unlock a mutex
bool mutex_unlock(mutex_t *mutex);

#endif /* INC_MUTEX_H_ */
