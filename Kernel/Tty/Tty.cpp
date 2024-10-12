#include <RemeoOS/Tty/Tty.hpp>
#include <stdint.h>
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
VgaTypes *Buffer = (VgaTypes *)0xB8000;
VgaTypes ThemeColor = VGA_COLOR_BLACK;

uint32_t TTY_COLUMN = 0;
uint16_t TTY_ROW = 0;

void Tty_Set_Theme(VgaTypes fg, VgaTypes bg) 
{
    ThemeColor = (bg << 4 | fg) << 8;
}
void Tty_Put_Char(char chr) 
{
    if (chr == '\n') {
        TTY_COLUMN = 0;
        TTY_ROW++;
    }
    else if (chr == '\r') 
    {
        TTY_COLUMN = 0;
    }
    else {
        *(Buffer + TTY_COLUMN + TTY_ROW * TTY_WIDTH) = (ThemeColor | chr);
        TTY_COLUMN++;
        if (TTY_COLUMN >= TTY_WIDTH) 
        {
            TTY_COLUMN = 0;
            TTY_ROW++;
        }
    }
    if (TTY_ROW >= TTY_HEIGHT) 
    {
        Tty_Scroll_Up();
        TTY_ROW--;
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