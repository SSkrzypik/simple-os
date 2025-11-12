#include "os/os.h"

#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

#include "stm32n6xx.h"

void task_idle(void);

void os_init() {
	SysTick->LOAD = (uint32_t) ((CPU_FREQUENCY / OS_TICK_PER_SECOND) - 1UL);
	SysTick->VAL = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;


	NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY);
	NVIC_SetPriority(PendSV_IRQn, PENDSV_IRQ_PRIORITY);

	task_create(task_idle, IDLE_TASK_NAME, MAX_TASK_PRIORITY, IDLE_TASK_STACK_SIZE);
}

void os_start() {
	__set_PSP(task_get_current()->psp);
	__set_CONTROL(__get_CONTROL() | 0x02);

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	while (1);
}

void task_idle(void) {
	while (1) {
		os_idle_hook();
		__NOP();
	}
}

__attribute__((weak)) void os_idle_hook(void) {}

__attribute__((weak)) void os_tick_hook(void) {}
