#include "pit.h"

/****************************************************************************
			Implementation of various functions that our
			pit will use ----- includes abstraction of registers also
*****************************************************************************/

#define I86_PIT_REG_COUNTER0 0x40
#define I86_PIT_REG_COUNTER1 0x41
#define I86_PIT_REG_COUNTER2 0x42
#define I86_PIT_REG_COUNTER3 0x43

// global variable that counts the system ticks

uint32_t _pit_ticks = 0;

// define an interrupt handler that is fired when the IRQ 0 is fired by the IDT

void interrupt _cdecl i86_pit_irq() {
	// macro to handle interrupt start
	intstart();
	// increment the global system tick count
	_pit_ticks++ ;
	interruptdone(0) ; // this sends the EOI command --> to be implemented in hal.h
	
	// return from interrupt handler after properly setting up the previous stack state
	intret();
}

void i86_pit_send_command(uint8_t cmd){
	outportb(I86_PIT_REG_COMMAND, cmd);
}

// write data into counter
void i86_pit_send_data(uint16_t data, uint8_t counter){
	uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 : 
	((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);
	
	outportb(port, data);
}

// read data from counter

uint16_t i86_pit_read_data(uint8_t counter){
	uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 : 
	((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);
	
	return inportb(port);
	
}

// initialize the pit now. remember we only need to install our interrupt in this case
// other than that there is nothing to initialize

void i86_pit_initialize(uint8_t irq, uint8_t irCodeSeg){
	i86_install_ir(irq, I86_IDT_DESC_PRESENT| I86_IDT_DESC_BIT32, irCodeSeg, i86_pit_irq);
	
}

// start up a counter --> any counter that is passed as a parameter to the function
// this function will also help in building up the ocw as we go along

void i86_pic_start_counter(uint32_t freq, uint8_t counter, uint8_t mode){
	if(freq == 0) return;
	
	uint16_t divisor = 1193180/ freq ;
	
	uint8_t ocw = 0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA ; // this indicates 
	//that we will put LSB then MSB into the data registers --> remember here MSB B means byte
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode ;
	
	i86_pit_send_command(ocw);
	// the data registers i.e. counter0 will now contain the count of ticks before the interrupt gets fired.
	i86_pit_send_data(divisor & 0xff, 0);
	i86_pit_send_data((divisor >> 8) & 0xff, 0);
	
	_pit_ticks = 0; // reset the global tick count
	
	
	
}
