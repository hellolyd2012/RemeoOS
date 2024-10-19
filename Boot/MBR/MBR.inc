;      Create at 2024.10.19 By hellolyd2012          ;
;      RemeoOS MBR  Config  

; Get 
Get:
    mov ah, 3
    mov bh, 1
    int 0x10

; Clean Screen
CleanScreen:
    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 0x184f

    int 0x10

; Print String
Print:
    mov ax, Message
    mov bp, ax

    mov cx, 6    
    mov ax, 0x1301
    mov bx, 0x2

    int 0x10

Message db "RemeoOS MBR" ; Sign String