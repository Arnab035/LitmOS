/******************************************************************************************
							Implementation of methods in PIC microcontroller
*******************************************************************************************/

//pic1 register port addresses abstracted

#define I86_PIC1_REG_COMMAND 0x20
#define I86_PIC1_REG_STATUS 0x20
#define I86_PIC1_REG_DATA 0x21
#define I86_PIC1_REG_IMR 0x21  // INTERRUPT MASK REGISTER

// pic2 register port addresses abstracted
#define I86_PIC2_REG_COMMAND 0xA0
#define I86_PIC2_REG_STATUS 0xA0
#define I86_PIC2_REG_DATA 0xA1
#define I86_PIC2_REG_IMR 0xA1

// set up initialization control word

#define I86_PIC_ICW1_MASK_IC4 0x1   // expect ICW4 bit
#define I86_PIC_ICW1_MASK_SNGL 0x2	// single pic or cascaded pic
#define I86_PIC_ICW1_MASK_ADI 0x4	// call address. usually ignored in x86 -> set to 0
#define I86_PIC_ICW1_MASK_LTIM 0x8	// operate in level triggered or edge triggered mode. 
#define I86_PIC_ICW1_MASK_INIT 0x10	// set if pic is to be initialized.


