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

// define command control bits for initialization control word 1 (icw1)

#define I86_PIC_ICW1_IC4_EXPECT 1
#define I86_PIC_ICW1_IC4_NO 0
#define I86_PIC_ICW1_SNGL_YES 2
#define I86_PIC_ICW1_SNGL_NO 0
#define I86_PIC_ICW1_ADI_CALLINTERVAL4 4
#define I86_PIC_ICW1_ADI_CALLINTERVAL8 0
#define I86_PIC_ICW1_LTIM_LEVELTRIGGERED 8
#define I86_PIC_ICW1_LTIM_EDGETRIGGERED 0
#define I86_PIC_ICW1_INIT_YES 0x10
#define I86_PIC_ICW1_INIT_NO 0

// define bit masks for icw 4

#define I86_PIC_ICW4_MASK_UPM 0x1		// mode 80x80 or MCS-80/86
#define I86_PIC_ICW4_MASK_AEOI 0x2		// automatic eoi
#define I86_PIC_ICW4_MASK_MS 0x4		// master/slave buffer to use ?
#define I86_PIC_ICW4_MASK_BUF 0x8		// whether to use buffer
#define I86_PIC_ICW4_MASK_SFNM 0x10		// sfnm (fully nested mode)-> used when you have a large number of cascaded PICs

//  command control bits for icw 4

#define I86_PIC_ICW4_UPM_86MODE 1
#define I86_PIC_ICW4_UPM_MCSMODE 0
#define I86_PIC_ICW4_AEOI_AUTOEOI 2
#define I86_PIC_ICW4_AEOI_NOAUTOEOI 0
#define I86_PIC_ICW4_MS_BUFFERMASTER 4
#define I86_PIC_ICW4_MS_BUFFERSLAVE 0
#define I86_PIC_ICW4_BUF_MODEYES 8
#define I86_PIC_ICW4_BUF_MODENO 0
#define I86_PIC_ICW4_SFNM_NESTEDMODE 0x10
#define I86_PIC_ICW4_SFNM_NOTNESTED 0


























