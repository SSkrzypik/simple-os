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