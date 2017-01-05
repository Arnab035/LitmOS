//***************************************************
// GDT.inc  -- GDT (global descriptor table) routines
//***************************************************

%ifndef  __GDT_INC_67343546FDCC56AAB872_INCLUDED__
%define  __GDT_INC_67343546FDCC56AAB872_INCLUDED__

bits 16

//***************************************************
// InstallGDT() - Install GDT
//***************************************************

InstallGDT:
	cli
	pusha
	lgdt [toc]
	sti
	popa
	ret
	
//***************************************************

gdt_data:      // zero descriptor
	dd 0
	dd 0
	
// gdt code:        // code descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
	
// gdt data:         // data descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
	
end_of_gdt:
	
toc:
	dw end_of_gdt - gdt_data - 1 // size of GDT
	dd get_data
	
%endif



