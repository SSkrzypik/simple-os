#include "os/os.h"
#include "stm32n6xx.h"

extern uint32_t task_count;
extern task_t *task_array[MAX_TASKS_COUNT];
extern uint32_t task_current;


void os_delay(uint32_t ticks) {
	enter_crit();

	task_array[task_current]->blocked_reason = BLOCKED_DELAY;
	task_array[task_current]->state = TASK_BLOCKED;
	task_array[task_current]->delay_ticks = ticks;
	exit_crit();
	os_yield();
}

void os_delay_ms(uint32_t miliseconds) {
	uint32_t ticks = (miliseconds * OS_TICK_PER_SECOND) / 1000;
	if (ticks == 0) {
		os_yield();
		return;
	}
	os_delay(ticks);
}

void SysTick_Handler(void) {
	for (uint32_t index = 0; index < task_count; index++) {
		if (task_array[index]->delay_ticks > 0) {
			task_array[index]->delay_ticks--;
		}
	}
	os_tick_hook();
	os_yield_from_irq();
}
