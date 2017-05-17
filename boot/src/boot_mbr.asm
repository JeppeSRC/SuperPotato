BITS 16

%define BREAK xchg bx, bx

jmp start

drive_number: db 0

drive:
drive_size: db 0x10
drive_zero: db 0
drive_numSectors: dw 0
drive_segment: dw 0x0
drive_offset: dw 0x0
drive_lba_low: dd 0
drive_lba_high: dd 0

print:
	push ax
	mov ah, 0x0E
	.print_loop:
	lodsb
	cmp al, 0
	je .done
	int 0x10
	jmp .print_loop

	.done:
	pop ax
	ret


start:
	BREAK
	mov ax, 0x7C0
	mov es, ax
	mov ds, ax
	mov ss, ax
	mov sp, 0xFFFF

	mov byte [drive_number], dl

	hlt
	
readSectors:
	push ax
	push dx
	mov ah, 0x42
	mov dl, byte [drive_number]
	int 0x13
	BREAK
	jc .readSectors_error
	pop dx
	pop ax
	ret

	.readSectors_error:
	mov si, readFail
	call print
	pop dx
	pop ax
	ret

readFail: db 'Failed to read sectors', 0

times 510 - ($ - $$) db 0
dw 0xAA55