/* Define memory regions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Define stack size */
STACK_SIZE = 4K; /* 4 KB stack, adjust as needed */

/* Define stack symbols globally */
_stack_top = ORIGIN(RAM) + LENGTH(RAM);
_stack_bottom = _stack_top - STACK_SIZE;

/* Define sections */
SECTIONS
{
    .isr_vector : 
    {
        *(.isr_vector)  /* Interrupt vector table */
    } > FLASH

    .text : 
    {
        *(.text*)       /* Code */
        *(.rodata*)     /* Read-only data */
        . = ALIGN(4);
    } > FLASH

    .data : 
    {
        *(.data*)       /* Initialized data */
        . = ALIGN(4);
    } > RAM AT > FLASH

    .bss : 
    {
        *(.bss*)        /* Uninitialized data */
        *(COMMON)       /* Common symbols */
        . = ALIGN(4);
    } > RAM

    /* Stack allocation */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        . = _stack_bottom;
    } > RAM
}

/* Define entry point */
ENTRY(Reset_Handler)
