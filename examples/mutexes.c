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