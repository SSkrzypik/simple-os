#include "os/os.h"

extern uint32_t task_count;
extern task_t *task_array[MAX_TASKS_COUNT];
extern uint32_t task_current;

void select_next_task() {
	enter_crit();
	//resuming inactive tasks
	bool is_active_task = false;
	for (uint32_t index = 0; index < task_count; index++) {
		if (task_array[index]->state == TASK_BLOCKED) {
			if (task_array[index]->delay_ticks == 0 && task_array[index]->blocked_reason == BLOCKED_DELAY ) {
				task_array[index]->state = TASK_RUNNING;
				is_active_task = true;
			}
		} else {
			is_active_task = true;
		}
	}

	if (!is_active_task) {
		task_current = 0; //idle task inde
		exit_crit();
		return;
	}

	//find highest priority
	task_priority_t highest = MAX_TASK_PRIORITY; // lowest priority
	for (uint32_t index = 0; index < task_count; index++) {
	    if (task_array[index]->state == TASK_RUNNING && task_array[index]->priority < highest) {
	        highest = task_array[index]->priority;
	    }
	}

	//Round-robin scheduler
	uint32_t start = (task_current + 1) % task_count;
	for (uint32_t offset = 0; offset < task_count; offset++) {
	    uint32_t index = (start + offset) % task_count;
	    if (task_array[index]->state == TASK_RUNNING && task_array[index]->priority == highest) {
	        task_current = index;
	        exit_crit();
	        return;
	    }
	}
	task_current = 0; //idle task index
	exit_crit();
}

void os_yield(void) {
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void os_yield_from_irq(void) {
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}
