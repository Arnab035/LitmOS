// to make things more object-oriented--> all input/output routines are moved into a stdio.inc file
// do not associate this with the C standard stdio library. They have almost nothing in common

//************************************************************************************************
// Stdio.inc -- all input output routines 
//************************************************************************************************

//************************************************************************************************;
// Puts16 () - Prints a null terminated string
//DS=>SI : 0 terminated string


%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__

bits 16

Puts16:
		pusha          //save registers
		
.Loop1:
		lodsb
		or al, al
		jz Puts16Done
		mov ah, 0eh
		int 10h
		jmp .Loop1

Puts16Done:
		popa
		ret
		
%endif          //__STDIO_INC_67343546FDCC56AAB872_INCLUDED__