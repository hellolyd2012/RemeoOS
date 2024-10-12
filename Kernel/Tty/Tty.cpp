// 创建于2024.10.12
// RemeoOS Kernel Tty

#include <RemeoOS/Tty/Tty.hpp>
#include <stdint.h>

#define TTY_WIDTH 80
#define TTY_HEIGHT 25

VgaTypes *Buffer = (VgaTypes *)0xB8000;
VgaTypes ThemeColor = VGA_COLOR_BLACK;

uint32_t Tty_Column = 0;
uint16_t Tty_Row = 0;

void Tty_Set_Theme(VgaTypes fg, VgaTypes bg) 
{
    ThemeColor = (bg << 4 | fg) << 8;
}

void Tty_Put_Char(char chr) 
{
    if (chr == '\n') {
        Tty_Column = 0;
        Tty_Row++;
    }
    else if (chr == '\r') 
    {
        Tty_Column = 0;
    }
    else {
        *(Buffer + Tty_Column + Tty_Row * TTY_WIDTH) = (ThemeColor | chr);
        Tty_Column++;
        if (Tty_Column >= TTY_WIDTH) 
        {
            Tty_Column = 0;
            Tty_Row++;
        }
    }
    if (Tty_Row >= TTY_HEIGHT) 
    {
        Tty_Scroll_Up();
        Tty_Row--;
    } 
}

void Tty_Put_Str(char* str) 
{
    while (*str != '\0') 
    {
        Tty_Put_Char(*str);
        str++;
    }
}

void Tty_Scroll_Up() 
{
    // TODO use memcpy
}

void Tty_Clear() 
{
    for (uint32_t x = 0; x < TTY_WIDTH; x++) 
    {
        for (uint32_t y = 0; y < TTY_HEIGHT; y++) 
        {
            *(Buffer + x + y * TTY_WIDTH) = ThemeColor;
        }
    }
}