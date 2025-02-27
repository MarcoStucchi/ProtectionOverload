# GDB commands

This document records the main GDB commands to launch and debug ARM program with QEMU and GDB

## QEMU

<pre> qemu-system-arm -M lm3s6965evb -kernel build/test_protection_overload_arm.elf -semihosting -S -gdb tcp::1234 </pre>

test 

#####

    qemu-system-arm -M lm3s6965evb -kernel build/test_protection_overload_arm.elf -semihosting -S -gdb tcp::1234