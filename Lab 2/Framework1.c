#include <fsl_device_registers.h>

void initLED(); void LEDOff(); void LEDOn(); void LEDToggle();

int main()
{
	PIT->CHANNEL[0].LDVAL = 0x30000; // set load value of zeroth PIT
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
	
	initLED();
}

void initLED()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[22] = PORT_PCR_MUX(001);
	PTB->PDDR |= (1 << 22);
	LEDOff();
}

void LEDOff()
{
	PTB->PCOR &= ~(1 << 22);
	PTB->PSOR |= (1 << 22);	
}

void LEDOn()
{
	PTB->PCOR |= (1 << 22);
	PTB->PSOR &= ~(1 << 22);
}

void LEDToggle()
{
	PTB->PSOR ^= (1 << 22);
	PTB->PCOR ^= (1 << 22);
}