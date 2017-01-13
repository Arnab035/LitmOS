/*************************************************************************************
                interface to our PIC microcontroller
                handling various kinds of interrupts
**************************************************************************************/

// these are interrupts for the PIC 1 (master pic)

// hide interrupts behind constants for easy portability and readability

#define I86_PIC_IRQ_TIMER 0
#define I86_PIC_IRQ_KEYBOARD 1
#define I86_PIC_IRQ_SERIAL2 3
#define I86_PIC_IRQ_SERIAL1 4
#define I86_PIC_IRQ_PARALLEL2 5
#define I86_PIC_IRQ_DISKETTE 6
#define I86_PIC_IRQ_PARALLEL1 7

// these are interrupts for the PIC 2 (slave pic)

#define I86_PIC_IRQ_CMOSTIMER 0
#define I86_PIC_IRQ_CGARETRACE 1
#define I86_PIC_IRQ_AUXILIARY 4
#define I86_PIC_IRQ_FPU 5
#define I86_PIC_IRQ_HDC 6

// define bits for operation command word 2 for the PIC (this is the main control word)

#define I86_PIC_OCW2_MASK_L1 1      //00000001 L1 interrupt level
#define I86_PIC_OCW2_MASK_L2 2		//00000010 L2 interrupt level
#define I86_PIC_OCW2_MASK_L3 4		//00000100 L3 interrupt level
#define I86_PIC_OCW2_MASK_EOI 0x20   //00100000 end of interrupt command
#define I86_PIC_OCW2_MASK_SL 0x40		//01000000 selection command
#define I86_PIC_OCW2_MASK_ROTATE 0x80	//10000000 rotation command

// define bits for operation control word 3 for the PIC

#define I86_PIC_OCW3_MASK_RIS 1
#define I86_PIC_OCW3_MASK_RIR 2
#define I86_PIC_OCW3_MASK_MODE 4
#define I86_PIC_OCW3_MASK_SMM 0x20
#define I86_PIC_OCW3_MASK_ESMM 0x40
#define I86_PIC_OCW3_MASK_D7 0x80



