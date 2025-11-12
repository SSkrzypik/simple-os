#ifndef INC_OS_OS_CRITICAL_H_
#define INC_OS_OS_CRITICAL_H_

#include "stm32n6xx.h"

// counts nested critical sections
static uint32_t crit_nesting = 0;

// disable interrupts, enter critical section
static inline void enter_crit(void) {
    __disable_irq();
    crit_nesting++;
}

// decrement nesting, enable interrupts if outermost
static inline void exit_crit(void) {
    if (crit_nesting > 0) {
        crit_nesting--;
        if (crit_nesting == 0) {
            __enable_irq();
        }
    }
}

#endif /* INC_OS_OS_CRITICAL_H_ */
