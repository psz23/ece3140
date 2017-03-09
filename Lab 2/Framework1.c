#include <fsl_device_registers.h>

const int TIMER_LENGTH = 0x01000000;

void initLED(void); void setLED(int);

int main(void)
{
	initLED();
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
	PIT->MCR &= ~(1 << 1); 					 // Enable PIT clocks (set 1st bit to 0)
	PIT->CHANNEL[0].LDVAL = TIMER_LENGTH; // set load value of PIT 0
	PIT->CHANNEL[0].TCTRL |= (1 << 0);	// Enable timer (begin counting)
	PIT->CHANNEL[0].TFLG |= (1 << 0);		// Clear timer done flag
		
	int isOn = 0;
	
	while(1)
	{
		if ((PIT->CHANNEL[0].TFLG & 1) == 1) { 
			setLED(~isOn); isOn = ~isOn;
			PIT->CHANNEL[0].TFLG |= (1 << 0);
		}
	}
}

void initLED(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[22] = PORT_PCR_MUX(001);
	PTB->PDDR |= (1 << 22);
	setLED(0);
}

void setLED(int set)
{
	if (set == 0) { PTB->PSOR |= (1 << 22);	}
	else { PTB->PCOR |= (1 << 22); }
}
