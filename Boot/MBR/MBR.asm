;      Create at 2024.10.19 By hellolyd2012          ;
;      RemeoOS MBR                                   ;

bits 16 ; 16 bit mode

%include "MBR.inc"

org 0x7c00 ; BIOS will load the MBR to 0x7c00

    mov ax, cs            
    mov ds, ax          
    mov es, ax   
    mov ss, ax 
    mov fs, ax
    mov sp, 0x7c00 

    call Print

    jmp $

; MBR Carge
times 510-($-$$) db 0 
db 0x55, 0xaa
