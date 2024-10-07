#include <Remeo/Tty/Tty.h>
#include <stdint.h>

#define TTY_WIDTH 80
#define TTY_HEIGHT 25

vga_atrributes *buffer = 0xb8000;
vga_atrributes ThemeColor = VGA_COLOR_BLACK;

uint32_t TTY_COLUMN = 0;
uint16_t TTY_ROW = 0;

void TtySetTheme(vga_atrributes fg, vga_atrributes bg) 
{
    ThemeColor = (bg << 4 | fg) << 8;
}
void TtyPutChar(char chr) {

    if (chr == '\n') {
        TTY_COLUMN = 0;
        TTY_ROW++;
    }
    else if (chr == '\r') 
    {
        TTY_COLUMN = 0;
    }
    else 
    {
        *(buffer + TTY_COLUMN + TTY_ROW * TTY_WIDTH) = (ThemeColor| chr);
        TTY_COLUMN++;
        if (TTY_COLUMN >= TTY_WIDTH) 
        {
            TTY_COLUMN = 0;
            TTY_ROW++;
        }
    }
    if (TTY_ROW >= TTY_HEIGHT) 
    {
        TtyScrollUp();
        TTY_ROW--;
    } 
}
void TtyPutStr(char* str) 
{
    while (*str != '\0') {
        TtyPutChar(*str);
        str++;
    }
}

void TtyScrollUp() 
{
    
}

void TtyClear() 
{
    for (uint32_t x = 0; x < TTY_WIDTH; x++) {
        for (uint32_t y = 0; y < TTY_HEIGHT; y++) {
            *(buffer + x + y * TTY_WIDTH) = ThemeColor;
        }
    }
}