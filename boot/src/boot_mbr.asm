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
drive_dap_size: db 0x10
drive_zero: db 0x00
drive_num: dw 0x00
drive_segment: dw 0x00
drive_offset: dw 0x00
drive_LBALow: dd 0x00
drive_LBAHigh: dd 0x00

%define SFS_ENTRY_SIZE 34
%define SFS_STRING_SIZE 255
%define TMP_WRITE_OFFSET 0x400

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

	mov [sfs_ClusterBytes], ax

	push ax
	mov dl, SFS_ENTRY_SIZE
	div dl

	mov [sfs_FileEntries], al
	pop ax

	mov dl, SFS_STRING_SIZE
	div dl

	mov [sfs_StringEntries], dl

	xor eax, eax
	mov al, byte [sfs_ReservedSectors]
	add eax, dword [sfs_TrackingSectors]

	mov [sfs_FirstCluster], eax

findFile:
	xor ax, ax
	mov al, [sfs_ClusterSize]
	mov byte [drive_num], al
	mov word [drive_segment], 0x7C0
	mov word [drive_offset], TMP_WRITE_OFFSET
	mov eax, dword [sfs_FirstCluster]
	mov dword [drive_LBALow], eax

	call readSectors

	mov si, TMP_WRITE_OFFSET

	movzx cx, byte [sfs_FileEntries]

	push si
	.findFile_loop:
		cmp byte [si], 0x01
		jne .findFile_loop_continue

		push cx
		push si
		add si, 0x09
		mov eax, dword [si]
		add si, 0x04
		movzx cx, byte [si]

		call getString

		mov di, filename

		repe cmpsb
		je readFile

		pop si
		pop cx

	.findFile_loop_continue:
		add si, SFS_ENTRY_SIZE
		loop .findFile_loop

	pop si
	add si, word [sfs_ClusterBytes]
	sub si, 0x08

;Get next cluster
	mov eax, dword [si]
	call clusterToSector

	mov cl, [sfs_FileEntries]

	mov dword [drive_LBALow], eax

	push si
	cmp eax, 0x00
	jne .findFile_loop

	hlt

readFile:
	pop si
	add sp, 2

	add si, 0x12

	mov eax, dword [si]
	add si, 0x08
	movzx ebx, word [sfs_ClusterBytes]
	sub ebx, 0x08
	div ebx

	mov eax, dword [si]
	mov dword [drive_LBALow], eax

	;TODO: readfile


;eax index
getString:
	push ax
	mov ax, word [sfs_ClusterBytes]
	add ax, TMP_WRITE_OFFSET

	mov word [drive_offset], ax
	mov eax, dword [sfs_FirstCluster]
	inc eax
	mov dword [drive_LBALow], eax

	pop ax
	movzx dx, byte [sfs_StringEntries]
	div dx

	call readSectors

	cmp ax, 0
	je .getString_skip

	push cx
	mov cx, ax

	mov si, word [drive_offset]

	.getString_loop:
		add si, word [sfs_ClusterBytes]
		sub si, 0x08
		mov eax, dword [si]
		call clusterToSector
		mov dword [drive_LBALow], eax
		loop .getString_loop

	pop cx

	.getString_skip:
	
	mov si, word [drive_offset]
	mov ax, SFS_STRING_SIZE
	mul dx

	add si, ax
	ret


readSectors:
	push ax
	push dx
	push si
	mov si, drive
	mov ah, 0x42
	mov dl, byte [drive_number]
	int 0x13
	BREAK
	jc .readSectors_error
	pop si
	pop dx
	pop ax
	ret

	.readSectors_error:
		hlt

;eax = cluster
;ret eax = sector
clusterToSector:
	mul byte [sfs_ClusterSize]
	add eax, dword [sfs_FirstCluster]
	ret




sfs_FileEntries: db 0x00
sfs_StringEntries: db 0x00
sfs_ClusterBytes: dw 0x00
sfs_FirstCluster: dd 0x00


filename: db 'boot_mbr2.bin', 0

times 510 - ($ - $$) db 0
dw 0xAA55