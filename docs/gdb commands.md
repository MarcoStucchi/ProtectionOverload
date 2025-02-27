# ARM debug commands with QEMU and GDB

This document summarizes the main QEMU and GDB commands to launch and debug emulation on ARM platforms.
This is due to a higher complexity associated with ARM programs, that will likely require often to engage in debugging sessions.

## Powershell

**Move to directory** (temporary command)
#####
     cd .\OneDrive\Documenti\NST\AutoTest\ProtectionOverload_SM\ 

## QEMU

The following commands are used to trigger QEMU emulation

**Launch QEMU** board lm3s6965evb
#####
    qemu-system-arm -M lm3s6965evb -kernel build/test_protection_overload_arm.elf -semihosting -S -gdb tcp::1234

The following options are included in the command: 
- -S → Starts QEMU but halts execution.
- -gdb tcp::1234 → Opens a GDB server on port 1234.

## GDB

GDB can be launched on a different PowerShell window

**Launch GDB**
#####
    arm-none-eabi-gdb build/test_protection_overload_arm.elf

**Connect to target**

#####
    target remote localhost:1234




