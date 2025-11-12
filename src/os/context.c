#include "os/os.h"

extern task_t *task_array[MAX_TASKS_COUNT];
extern uint32_t task_current;

extern void select_next_task();

volatile bool scheduler_initialized = false;

void save_current_psp(uint32_t psp);
uint32_t get_current_psp();

__attribute__((naked)) void PendSV_Handler(void) {
	__asm volatile("PUSH {LR}");

	__asm volatile("MRS R0, PSP");
	__asm volatile("STMDB R0!, {R4-R11}");

	__asm volatile("LDR R12, =scheduler_initialized");
	__asm volatile("LDRB R12, [R12]");
	__asm volatile("CMP R12, #0");
	__asm volatile("BEQ skip_context_switch");

	__asm volatile("BL save_current_psp");
	__asm volatile("BL select_next_task");

	__asm volatile("skip_context_switch:");

	__asm volatile("LDR R12, =scheduler_initialized");
	__asm volatile("MOV R0, #1");
	__asm volatile("STRB R0, [R12]");


	__asm volatile("BL get_current_psp");
	__asm volatile("LDMIA R0!, {R4-R11}");
	__asm volatile("MSR PSP, R0");

	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
}

void save_current_psp(uint32_t psp) {
	task_array[task_current]->psp = psp;
}

uint32_t get_current_psp() {
	return task_array[task_current]->psp;
}


