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

extern int main();

/* Reset handler */
void Reset_Handler(void) {

    /* Initialize data and bss */
    extern uint32_t _data_loadaddr, __data_start__, __data_end__, __bss_start__, __bss_end__;
    uint32_t *src, *dst;

    src = &_data_loadaddr;
    dst = &__data_start__;
    while (dst < &__data_end__) {
        *dst++ = *src++;
    }

    dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }

    /* Call main() */
    main();

    /* Hang */
    while (1);
}