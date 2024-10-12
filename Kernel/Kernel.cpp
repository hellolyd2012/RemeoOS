#include <RemeoOS/Tty/Tty.hpp>
#include <RemeoOS/Arch/X86/Gdt.hpp>

extern "C" void _KernelInit()
{
    _Init_Gdt();
    Tty_Set_Theme(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
}

extern "C" void _KernelMain()
{
    Tty_Put_Str("Weclome RemeoOS!\n");
}