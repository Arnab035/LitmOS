;---------------------------------------
; get memory map from BIOS
;---------------------------------------

; define the structure of each memory map entry

struc MemoryMapEntry
	.baseAddress resq 1 ; base address of address range
	.length resq 1	; length of address range in bytes
	.type resd 1 ; type of address range 
	.acpi_null resd 1 ; reserved ... I don't know ???


BiosGetMemoryMap:
	pushad
	xor ebx, ebx
	xor bp, bp
	mov edx, 'SMAP'
	mov eax, 0xe820
	mov ecx, 24   ; memory map entry should be 24 bytes
	int 0x15;      ; get me the first entry
	jc .error
	cmp eax, 'SMAP'
	jne .error
	test ebx, ebx
	je .error
	; test sets the zero flag if the result of the 'AND' operation is zero. 2 operands
	; are equal and their bitwise and is 0 iff both the operands are zero. je(tests the zero flag and jumps if the flag is set)
	; 'je' can be thought to be an alias of 'jz'
	jmp .start
.next_entry:
	mov edx, 'SMAP'
	mov eax, 0xe820
	mov ecx, 24
	int 0x15
.start:
	jcxz .skip_entry
.no_text:
	mov ecx, [es:di + MemoryMapEntry.length] ; cannot understand now ??
	test ecx, ecx
	jne short .good_entry
	mov ecx, [es:di + MemoryMapEntry.length + 4]
	jecxz .skip_entry
.good_entry:
	inc bp
	add di, 24
.skip_entry:
	cmp bx, 0
	jne .next_entry
	jmp .done
.error
	stc
.done:
	popa
	ret