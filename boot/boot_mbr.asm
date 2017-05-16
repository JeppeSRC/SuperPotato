BITS 16

%define BREAK xchg bx, bx

;ORG 0x7C00

jmp start

drive_number: db 0

drive: db 0x10
drive_zero: db 0
drive_numSectors: dw 0
drive_segment: dw 0
drive_offset: dw 0
drive_lba_low: dd 0
drive_lba_high: dd 0

print:
	push ax
	.print_loop:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0E
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

	jmp 0x7C0:bleh

	bleh:

	mov byte [drive_number], dl

	mov word [drive_numSectors], 1
	mov word [drive_segment], 0x7E0
	mov word [drive_offset], 0
	mov word [drive_lba_low], 1
	mov word [drive_lba_high], 0
	mov si, drive
	call readSectors



	hlt
	
readSectors:
	push ax
	push dx
	clc
	mov ah, 0x42
	mov dl, byte [drive_number]
	xor al, al
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