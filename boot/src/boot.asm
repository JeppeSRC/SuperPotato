BITS 16

%define BREAK xchg bx, bx

jmp start


%include "print.inc"


start:
BREAK
mov ax, 0x7E0
mov es, ax
mov fs, ax
mov ds, ax

mov si, message
call print

hlt

message: db 'Loaded!', 0