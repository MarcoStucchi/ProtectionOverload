#include <stdint.h>

/* Linker script symbols */
extern uint32_t _stack_top;
extern uint32_t __data_start__, __data_end__, _data_loadaddr;
extern uint32_t __bss_start__, __bss_end__;

void Reset_Handler(void);
void Default_Handler(void) { while (1); }

/* Core exception handlers */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Vector Table */
__attribute__((section(".isr_vector")))
void (*const g_vectors[])(void) = {
    (void (*)(void))&_stack_top,      /* Initial stack pointer */
    Reset_Handler,                     /* Reset handler */
    NMI_Handler,                       /* NMI */
    HardFault_Handler,                 /* HardFault */
    MemManage_Handler,                 /* MemManage */
    BusFault_Handler,                  /* BusFault */
    UsageFault_Handler,                /* UsageFault */
    0, 0, 0, 0,                        /* Reserved */
    SVC_Handler,                       /* SVCall */
    DebugMon_Handler,                  /* Debug Monitor */
    0,                                 /* Reserved */
    PendSV_Handler,                    /* PendSV */
    SysTick_Handler,                   /* SysTick */
    /* Add more peripheral handlers as needed */
};

extern int main(void);

/* Reset handler */
void Reset_Handler(void) {
    uint32_t *src, *dst;

    /* Copy data section from flash to RAM */
    src = &_data_loadaddr;
    dst = &__data_start__;
    while (dst < &__data_end__) {
        *dst++ = *src++;
    }

    /* Zero fill the BSS section */
    dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }

    /* Enable FPU if needed */
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* Set CP10 and CP11 Full Access */
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
    #endif

    /* Call main() */
    main();

    /* Infinite loop if main returns */
    while (1);
}