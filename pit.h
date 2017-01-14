/*****************************************************************************
					Interface for the programmable interrupt timer
					--> we are focusing only on the interrupt timer as 
					our hardware that sends interrupts
*****************************************************************************/

// these are the bit masks that we use

#include <stdint.h>

#define I86_PIT_OCW_MASK_BINCOUNT 1
#define I86_PIT_OCW_MASK_MODE 0xE
#define I86_PIT_OCW_MASK_RL 0x30
#define I86_PIT_OCW_MASK_COUNTER 0xC0

// below are the command control bits that are used
// to supplement the mask bits 

#define I86_PIT_OCW_BINCOUNT_BINARY 0
#define I86_PIT_OCW_BINCOUNT_BCD 1
#define I86_PIT_OCW_MODE_TERMINALCOUNT 0
#define I86_PIT_OCW_MODE_ONESHOT 0x2
#define I86_PIT_OCW_MODE_RATEGEN 0x4
#define I86_PIT_OCW_MODE_SQUAREWAVEGEN 0x6
#define I86_PIT_OCW_MODE_SOFTWARETRIG 0x8
#define I86_PIT_OCW_MODE_HARDWARETRIG 0xA
#define I86_PIT_OCW_RL_LATCH 0
#define I86_PIT_OCW_RL_LSBONLY 0x10
#define I86_PIT_OCW_RL_MSBONLY 0x20
#define I86_PIT_OCW_RL_DATA 0x30
#define I86_PIT_OCW_COUNTER_0 0
#define I86_PIT_OCW_COUNTER_1 0x40
#define I86_PIT_OCW_COUNTER_2 0x80

// functions to be defined here...
