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