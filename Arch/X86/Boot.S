// 创建于2024.10.12
// RemeoOS X86 BootLoader

#include <RemeoOS/Arch/X86/MultiBoot.hpp>

.extern _KernelInit
.extern _KernelMain

.section .MultiBoot
    .long MAGIC
    .long ALi
    .long CHECKSUM(ALi)

.section .bss
    .align 16
    StackBottom:
    StackTop:
        .skip 16318, 0

.section .text
    .global boot
    .type boot, @function
    boot:
        movl $StackTop, %esp

        call _KernelInit

        subl $0x6, %esp
        movl $_Gdt, 2(%esp)
        movw _Gdt_Limit, %ax
        movw %ax, (%esp)
        lgdt (%esp)
        addl $0x6, %esp
        
        movw $0x10, %cx
        movw %cx, %es
        movw %cx, %ds
        movw %cx, %fs
        movw %cx, %gs
        movw %cx, %ss

        pushw $0x08
        pushl $_After_Gdt
        retf

    _After_Gdt:
        pushl %ebx

        call _KernelMain

        cli
    Stop:
        hlt
        jmp Stop