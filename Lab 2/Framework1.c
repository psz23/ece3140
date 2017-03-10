#include <fsl_device_registers.h>

#define LED_INTERVAL 0x01000000

void initLED(void); void setLED(int);

void initTimers(void); void runTimer(int, int); int checkTimer(int); void resetTimer(int);  

int main(void)
{
	initLED(); initTimers(); runTimer(0, LED_INTERVAL);
	
	int isOn = 0;
	
	while(1) {
		if (checkTimer(0) == 1) { 
			setLED(~isOn); isOn = ~isOn; resetTimer(0);
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

void initTimers(void)
{
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
	PIT->MCR &= ~(1 << 1); 			 // Enable PIT clocks (set 1st bit to 0)	
}

void runTimer(int num, int length)
{
	PIT->CHANNEL[num].LDVAL = length; 		// set load value of timer
	PIT->CHANNEL[num].TCTRL |= (1 << 0);	// Enable timer (begin counting)
	PIT->CHANNEL[num].TFLG |= (1 << 0);		// Clear timer done flag		
}

int checkTimer(int num)
{
	return (PIT->CHANNEL[num].TFLG & (1 << 0));
}

void resetTimer(int num)
{
	PIT->CHANNEL[num].TFLG |= (1 << 0);
}
