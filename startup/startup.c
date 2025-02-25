#include <stdint.h>

// Define the stack (allocated in RAM)
#define STACK_SIZE  1024
static uint8_t stack[STACK_SIZE] __attribute__((aligned(8), used, section(".stack")));

// Forward declaration of the Reset Handler
void Reset_Handler(void);

// Minimal vector table (SP + Reset Handler)
__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))(&stack[STACK_SIZE]),  // Stack pointer (top of stack)
    Reset_Handler                          // Reset Handler
};

// Dummy Reset Handler (calls `main`)
void Reset_Handler(void) {
    extern int main(void);
    main();
    while (1);  // Stay here if `main` returns
}
