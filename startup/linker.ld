/* Define memory regions */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rw) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Define stack size */
STACK_SIZE = 4K; /* 4 KB stack, adjust as needed */

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

    /* Exception Index Table (Required for ARM unwinder) */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > FLASH    

    .data (NOLOAD): 
    {   
        _data_loadaddr = LOADADDR(.data);   
        __data_start__ = .;
        *(.data*)       /* Initialized data */
        __data_end__ = .;
        . = ALIGN(4);
    } > RAM AT > FLASH

    .bss (NOLOAD): 
    {
        __bss_start__ = .;
        *(.bss*)        /* Uninitialized data */
        *(COMMON)       /* Common symbols */
        . = ALIGN(4);
        __bss_end__ = .;
        _end = .;    /* Define _end symbol */
        end = .;     /* Alternative name */
    } > RAM

    /* Stack allocation */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _stack_top = ORIGIN(RAM) + LENGTH(RAM);
        . = STACK_SIZE;
        _stack_bottom = .;
    } > RAM
}

/* Define entry point */
ENTRY(Reset_Handler)
