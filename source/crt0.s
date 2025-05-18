.section .text
.global _start
.arm

_start:
    b startup  @ Jump to the startup code

.global __boot_method, __slave_number
__boot_method:
    .word 0
__slave_number:
    .word 0

.global start_vector
start_vector:
    b doNothing @ Start vectors
    .space 0x9C   @ Reserved space after startup

startup:
    mov r0, #0x12     @ Switch to IRQ mode
    msr cpsr, r0
    ldr sp, =0x3007FA0  @ Set IRQ stack

    mov r0, #0x13     @ Switch to SVC mode
    msr cpsr, r0
    ldr sp, =0x3007FE0  @ Set SVC stack

    mov r0, #0x1F     @ Switch to System mode
    msr cpsr, r0
    ldr sp, =0x3007F00  @ Set System stack

    ldr r0, =__bss_start  @ Clear BSS section
    ldr r1, =__bss_end
    sub r1, r1, r0
    bl ClearMemory

    @ Branch to the main function
    bl main

    @ Branch to the doNothing function if main returns
    b doNothing

doNothing:
    b doNothing @ Infinite loop

@ Clear memory routine
ClearMemory:
    mov r2, #0
    cmp r1, #0
    bxeq lr

clearLoop:
    strb r2, [r0]
    add r0, r0, #1
    subs r1, r1, #1
    bne clearLoop
    bx lr