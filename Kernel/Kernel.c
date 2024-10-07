#include <Remeo/Tty/Tty.h>

void _KernelInit() {
    // TODO
}

void _KernelMain(void* info_table) {
    // TODO
    TtySetTheme(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    TtyPutStr("Hello kernel world!\nThis is second line.");
}