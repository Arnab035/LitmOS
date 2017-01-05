bits 16   ; I am starting off in real mode

; Remember the memory map - 0x500 through 0x7bff is unused above the BIOS data
; area. We are loaded at 0x500 (0x50 : 0).

org 0x500
jmp main  ; go to start

;**************************************************
; Preprocessor Directives
;**************************************************

%include "stdio.inc"
%include "GDT.inc"

;**************************************************
; Data section
;**************************************************

LoadingMsg db "Preparing to load operating system", 0x0D, 0x0A, 0x00

;**************************************************
; STAGE 2 ENTRY POINT
;      - store BIOS information
;      - Load Kernel
;      - Install GDT, go into protected mode
;      - Jump to stage 3
; *************************************************


main:
	;-----------------------------------
	; Set up segments and stack
	;-----------------------------------
	
	cli
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xFFFF      ; stack area starts from 0x9000 to 0xFFFF
	sti
	
	;-----------------------------------
	; Print loading message
	;-----------------------------------
	
	mov si, LoadingMsg
	call Puts16
	
	;-----------------------------------
	; Install GDT
	;-----------------------------------
	
	call InstallGDT
	
	;-----------------------------------
	; Go into Protected Mode
	;-----------------------------------
	
	cli
	mov eax, cr0
	or eax, 1              ; set bit0 in cr0- enter pmode
	mov cr0, eax
	
	jmp 08h:Stage3          ; once you go into protected mode, talk in terms
							; of GDT and the different  descriptors
							; here you access the code descriptor (08h offset from start of GDT)
							; far jump makes sure code segment is 08h and instruction pointer
							; is stage3
							
	
	;*************************************************
	; Entry point for stage 3
	;*************************************************
	
bits 32                       ; 32-bit world now

Stage3:

	;------------------------------------------;
	;  Set Registers ;
	;------------------------------------------;
	
	mov ax, 0x10
	mov ds, ax             ; all of data segment, extra segment, stack segment point to data descriptor now
						   ; remember all of this is relative to the base address of GDT
						   ; why ? because addresses will now be referenced in descriptor:address format
						   ; instead of segment:offset format -- that is why in the segment registers
						   ; store the accurate addresses of the descriptors
	mov es, ax
	mov ss, ax
	mov esp, 9000h         ; stack pointer begins from 9000h
	
;*********************************************
; Stop execution
;*********************************************

STOP:
	cli
	hlt
	
	
	
	
	
	
	




