#include "os/os.h"

#include "stdlib.h"
#include "string.h"

task_t *task_array[MAX_TASKS_COUNT] = {0};
uint32_t task_count = 0;
uint32_t task_current = 0;


task_t *task_create(void (*handler)(void), const char *task_name, task_priority_t priority, uint32_t stack_size) {
	task_t *task = malloc(sizeof(task_t));
	if (task == NULL) {
	    return NULL;
	}

	uint32_t *stack = malloc(stack_size * sizeof(uint32_t));
	if (stack == NULL) {
	    free(task);
	    return NULL;
	}

	task_create_static(handler, task_name, priority, stack_size, stack, task);

	task->alloc_type = TASK_ALLOC_DYNAMIC; //override alloc_type

	return task;
}


void task_create_static(void (*handler)(void), const char *task_name, task_priority_t priority, uint32_t stack_size, uint32_t *stack, task_t *task_buffer) {
	if (task_buffer == NULL || stack == NULL) {
		return;
	}

	if (task_count >= MAX_TASKS_COUNT) {
		return;
	}

	if (stack_size > MAX_STACK_SIZE) {
		stack_size = MAX_STACK_SIZE;
	}

	task_buffer->stack_base = stack;
	for (uint32_t index = 0; index < stack_size; index++) {
		stack[index] = 0xDEADBEEF;
	}
	stack += (stack_size - 1);
	*(--stack) = 0x01000000u; // xPSR
	*(--stack) = (uint32_t) handler; // PC
	*(--stack) = 0xFFFFFFFDu; // LR
	*(--stack) = 0x12121212u; // R12
	*(--stack) = 0x03030303u; // R3
	*(--stack) = 0x02020202u; // R2
	*(--stack) = 0x01010101u; // R1
	*(--stack) = 0x00000000u; // R0
	*(--stack) = 0x11111111u; // R11
	*(--stack) = 0x10101010u; // R10
	*(--stack) = 0x09090909u; // R9
	*(--stack) = 0x08080808u; // R8
	*(--stack) = 0x07070707u; // R7
	*(--stack) = 0x06060606u; // R6
	*(--stack) = 0x05050505u; // R5
	*(--stack) = 0x04040404u; // R4

	strncpy(task_buffer->name, task_name, MAX_TASK_NAME_LENGTH - 1);
	task_buffer->name[MAX_TASK_NAME_LENGTH - 1] = '\0';
	if (priority > MAX_TASK_PRIORITY) {
		priority = MAX_TASK_PRIORITY;
	}

	task_buffer->priority = priority;
	task_buffer->alloc_type = TASK_ALLOC_STATIC;
	task_buffer->handler = handler;
	task_buffer->psp = (uint32_t) stack;
	task_buffer->state = TASK_RUNNING;
	task_buffer->delay_ticks = 0;
	task_buffer->id = task_count;

	task_array[task_count] = task_buffer;
	task_count++;
}

void task_delete(task_t *task) {
	if (task == NULL) return;

    enter_crit();

    for (uint32_t index = 0; index < task_count; index++) {
        if (task_array[index] == task) {
            task_array[index] = NULL;
            break;
        }
    }

    task->state = TASK_SUSPENDED;

    if (task->alloc_type == TASK_ALLOC_DYNAMIC) {
        if (task->stack_base) {
            free(task->stack_base);
            task->stack_base = NULL;
        }
        free(task);
    }


    exit_crit();
}

void task_suspend(task_t *task) {
	if (task == NULL) return;

	enter_crit();
	task->state = TASK_SUSPENDED;
	//remove_from_ready_queue(task); TODO
	exit_crit();
}

void task_resume(task_t *task) {
	if (task == NULL) return;

	enter_crit();
	task->state = TASK_READY;
	//insert_into_ready_queue(task); TODO
	exit_crit();
}

task_t *task_get_current(void) {
	return task_array[task_current];
}
