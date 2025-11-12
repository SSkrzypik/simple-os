# Simple-OS — Minimal Preemptive RTOS for ARM Cortex-M

Simple-OS is a lightweight preemptive real-time operating system written from scratch in C and ARM assembly.  
It was designed for educational and experimental purposes — to understand how a real RTOS kernel works under the hood.

## Features

- 🕒 Preemptive scheduler with priority-based round-robin
- 🔄 Context switching using PendSV and PSP/MSP separation
- 🔐 Mutexes and semaphores for synchronization
- 🧵 Task management
  - Task creation and deletion
  - Blocking, ready, and running states
- ⏱️ `os_delay()` based on SysTick timer
- ⚙️ Critical section management with interrupt disabling
- 🚀 C and inline ARM assembly

## Example usage

### Mutexes

This example demonstrates how to use a mutex to synchronize two tasks.
```c
#include "os/os.h"

mutex_t mutex;

void task_first(void);
void task_second(void);

void example_mutex(void) {
    os_init();

    task_create(task_first, "first_task", 1, 128);
    task_create(task_second, "second_task", 1, 128);

    mutex = mutex_create();

    os_start();
}

void task_first(void) {
    char *message = "task first\n\r";
    while (1) {
        mutex_lock(&mutex, OS_MAX_DELAY);

        uart_send_string(message);

        mutex_unlock(&mutex);
    }
}

void task_second(void) {
    char *message = "task second\n\r";
    while (1) {
        mutex_lock(&mutex, OS_MAX_DELAY);

        uart_send_string(message);

        mutex_unlock(&mutex);
    }
}
```
### Semaphores

This example uses semaphores to notify and synchronize tasks.
```c
#include "os/os.h"

void task_first(void);
void task_second(void);

semaphore_t semaphore;

void example_semaphore(void) {
    os_init();

    task_create(task_first, "first_task", 1, 128);
    task_create(task_second, "second_task",1, 128);

    semaphore = semaphore_create_binary();

    os_start();
}


void task_first(void) {
    char *message = "semaphore taken\n\r";
    while (1) {
        semaphore_take(&semaphore, OS_MAX_DELAY);
        uart_send_string(message);
    }
}

void task_second(void) {
    while (1) {
        semaphore_give(&semaphore);
        os_delay_ms(1000);
    }
}
```
