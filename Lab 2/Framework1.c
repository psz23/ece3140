#include <fsl_device_registers.h>

const int TIMER_LENGTH = 0x01000000;

void initLED(void); void LEDOff(void); void LEDOn(void); void LEDToggle(void);

int main(void)
{
	initLED();
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
	PIT->MCR &= ~(1 << 1); 					 // Enable PIT clocks (set 1st bit to 0)
	PIT->CHANNEL[0].LDVAL = TIMER_LENGTH; // set load value of zeroth PIT
	PIT->CHANNEL[0].TCTRL |= (1 << 0);	// Enable timer
	PIT->CHANNEL[0].TFLG |= (1 << 0);		// Clear timer done flag
			
	while(1)
	{
		int x = PIT->CHANNEL[0].CVAL;
		int y = PIT->CHANNEL[0].TFLG;
		if ((PIT->CHANNEL[0].TFLG & 1) == 1) 
		{ 
			LEDOn(); 
		}
	}
}

void initLED(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[22] = PORT_PCR_MUX(001);
	PTB->PDDR |= (1 << 22);
	LEDOff();
}

void LEDOff(void)
{
	PTB->PCOR &= ~(1 << 22);
	PTB->PSOR |= (1 << 22);	
}

void LEDOn(void)
{
	PTB->PCOR |= (1 << 22);
	PTB->PSOR &= ~(1 << 22);
}

void LEDToggle(void)
{
	PTB->PSOR ^= (1 << 22);
	PTB->PCOR ^= (1 << 22);
}
