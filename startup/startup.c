#include <stdint.h>

/* Linker script symbols */
extern uint32_t _stack_top;
extern void Reset_Handler(void);

/* Weak definitions for system handlers */
void Default_Handler(void) { while (1); }
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Vector Table */
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_stack_top,  /* Stack pointer */
    (uint32_t *)Reset_Handler, /* Reset handler */
    (uint32_t *)NMI_Handler,   /* NMI */
    (uint32_t *)HardFault_Handler, /* HardFault */
    /* Add more handlers as needed */
};
