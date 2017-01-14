/******************************************************************************************
							Implementation of methods in PIC microcontroller
*******************************************************************************************/

#include "pic.h"


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

// useful functions defined 

/* send command to the appropriate pic register */
inline void i86_pic_send_command(uint8_t cmd, uint8_t picNum) {
	if(picNum > 1) return;
	
	uint8_t reg = (picNum == 1) ? I86_PIC2_REG_COMMAND : I86_PIC1_REG_COMMAND;
	outportb(reg, cmd);             // defined in hal.h

}

/* write data into the appropriate pic data register --> this is actually the interrupt mask register */

inline void i86_pic_send_data(uint8_t data, uint8_t picNum) {
	if(picNum > 1) return ;
	
	uint8_t reg = (picNum==1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA ;
	outportb(reg, data);

}

/* read data from the appropriate pic data register*/

inline uint8_t i86_pic_read_data(uint8_t picNum){
	if(picNum > 1) return;
	
	uint8_t reg = (picNum==1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA ;
	return inportb(reg);
}


/* initialize pic using the initialization control words -- discussed */

void i86_pic_initialize(uint8_t base0, uint8_t base1){
	
	// interestingly enough only the first icw is a command byte --> the rest are data bytes
	uint8_t icw = 0;
	
	icw = (icw & ~I86_PIC_ICW1_MASK_INIT) | I86_PIC_ICW1_INIT_YES ;
	
	icw = (icw & ~I86_PIC_ICW1_MASK_IC4) | I86_PIC_ICW1_IC4_EXPECT ;
	i86_pic_send_command(icw, 0);
	
	i86_pic_send_command(icw, 1);
	
	// send the icw 2. this byte contains the base interrupt number for the pic
	
	i86_pic_send_data(base0, 0);
	i86_pic_send_data(base1, 0);
	
	// send the icw3. icw3 for master pic is the irq that connects to the secondary pic in binary format.
	//icw3 for secondary pic is the irq that connects to the master pic in decimal format
	
	i86_pic_send_data(0x04, 0);
	i86_pic_send_data(0x02, 0);
	
	// send the icw4 --> make the mode as x86.
	
	icw = (icw & ~I86_PIC_ICW4_MASK_UPM) | I86_PIC_ICW4_UPM_86MODE ;
	
	i86_pic_send_data(icw, 0);
	i86_pic_send_data(icw, 1);
	
}































