BITS 16

ORG 0x7E00

jmp start


%include "print.inc"


start:

mov si, message
call print

hlt

message: db 'Loaded!', 0