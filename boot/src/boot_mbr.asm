BITS 16

%define BREAK xchg bx, bx

jmp start

drive_number: db 0

sfs_ID: 				dw 0x00
sfs_Version: 			dw 0x00
sfs_Reserved:			dw 0x00
sfs_Label:				times 16 db 0x00
sfs_ReservedSectors:	db 0x00
sfs_SectorSize:			dw 0x00
sfs_ClusterSize:		db 0x00
sfs_TrackingSectors:	dq 0x00
sfs_DatagSectors:		dq 0x00

drive:
drive_dap_size: 		db 0x10
drive_zero:				db 0x00
drive_num:				dw 0x00
drive_segment:			dw 0x00
drive_offset:			dw 0x00
drive_LBALow:			dd 0x00
drive_LBAHigh:			dd 0x00

%define SFS_ENTRY_SIZE 34

%if 0
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
%endif

start:
	BREAK
	mov ax, 0x7C0
	mov es, ax
	mov ds, ax
	mov ss, ax
	mov sp, 0xFFFF

	mov byte [drive_number], dl

	xor ax, ax
	mov ax, word [sfs_SectorSize]
	mul byte [sfs_ClusterSize]

	mov dl, SFS_ENTRY_SIZE
	div dl

	xor eax, eax
	mov al, byte [sfs_ReservedSectors]
	add eax, dword [sfs_TrackingSectors]



	hlt

readSectors:
	push ax
	push dx
	mov si, drive
	mov ah, 0x42
	mov dl, byte [drive_number]
	int 0x13
	BREAK
	jc .readSectors_error
	pop dx
	pop ax
	ret

	.readSectors_error:
		hlt

filename: db 'boot/boot_mbr2.bin'

times 510 - ($ - $$) db 0
dw 0xAA55