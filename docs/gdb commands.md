# ARM debug commands with QEMU and GDB

This document summarizes the main QEMU and GDB commands to launch and debug emulation on ARM platforms.
This is due to a higher complexity associated with ARM programs, that will likely require often to engage in debugging sessions.

## QEMU

The following commands are used to trigger QEMU emulator

**Launch QEMU** board lm3s6965evb
#####
    qemu-system-arm -M lm3s6965evb -kernel build/test_protection_overload_arm.elf -semihosting -S -gdb tcp::1234


##