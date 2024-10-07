#include "MultiBoot.h"

.section .MultiBoot
    .long MAGIC
    .long ALI
    .long CHECKSUM(ALI)

.section .bss
    .align 16
    stack_bottom:
        .skip 16318, 0
    stack_top:

.section .text
    .global boot_
    .type boot_, @function

    boot_:
        movl $stack_top, %esp
        /* 
            TODO: kernel init
                1. Load GDT
                2. Load IDT
                3. Enable paging
        */
        call _KernelInit

        pushl %ebx
        call _KernelMain

        cli
    Stop:
        hlt
        jmp Stop