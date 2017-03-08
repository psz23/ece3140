
#include <fsl_device_registers.h>

int main()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable the clock to port B
	PORTB->PCR[22] = PORT_PCR_MUX(001); // Set up PTB18 as GPIO
	PTB->PDDR |= (1 << 22); 						// enable data direction out on 22
	
	PTB->PCOR |= (1 << 22); 
	PTB->PSOR |= (0 << 22);
}
