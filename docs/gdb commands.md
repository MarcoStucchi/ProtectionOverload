# GDB commands

This document records the main GDB commands to launch and debug ARM program with QEMU and GDB

## QEMU

The following commands are used to trigger QEMU emulator

#####
    qemu-system-arm -M lm3s6965evb -kernel build/test_protection_overload_arm.elf -semihosting -S -gdb tcp::1234