#include <fsl_device_registers.h>

// timer clocks run at 20 MHz
#define ONE_SECOND 0x01312D00 // ~1 second
#define TENTH_SECOND 0x001E8480 // ~0.1 seconds
#define ADD_CYCLES_FIX 6

#define LEDB_PIN 21
#define LEDG_PIN 26

void PIT0_IRQHandler(void);
void initLEDB(void); 
void setLEDB(int);
void initLEDG(void);
void setLEDG(int);
void initPITs(void);
void runTimer(int, int, int);
void stopTimer(int);
void resetTimer(int); 

volatile int LEDG_STATUS = 0;

int main(void)
{	
	initLEDB(); initLEDG(); initPITs(); 
	
	NVIC_EnableIRQ(PIT0_IRQn); 
	runTimer(0, ONE_SECOND, 1);
	
	int LEDB_STATUS = 0;
	
	while(1) 
	{ 
		int i; for (i = 0; i < ONE_SECOND / ADD_CYCLES_FIX; i++) { }
		setLEDB(~LEDB_STATUS); LEDB_STATUS = ~LEDB_STATUS;	
	}
}

void PIT0_IRQHandler(void)
{
	resetTimer(0); 
	
	LEDG_STATUS = ~LEDG_STATUS;
	setLEDG(LEDG_STATUS);
	
	runTimer(0, LEDG_STATUS ? TENTH_SECOND : ONE_SECOND, 1);
}

void initLEDB(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[LEDB_PIN] = PORT_PCR_MUX(001);
	PTB->PDDR |= (1 << LEDB_PIN);
	setLEDB(0);
}

void setLEDB(int set)
{
	if (set == 0) { PTB->PSOR |= (1 << LEDB_PIN);	}
	else { PTB->PCOR |= (1 << LEDB_PIN); }
}

void initLEDG(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[LEDG_PIN] = PORT_PCR_MUX(001);
	PTE->PDDR |= (1 << LEDG_PIN);
	setLEDG(0);
}

void setLEDG(int set)
{
	if (set == 0) { PTE->PSOR |= (1 << LEDG_PIN);	}
	else { PTE->PCOR |= (1 << LEDG_PIN); }
}

void initPITs(void)
{
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
	PIT->MCR &= ~(1 << 1); 			 // Enable PIT clocks (set 1st bit to 0)	
}

void runTimer(int num, int length, int interrupts)
{
	stopTimer(num);
	PIT->CHANNEL[num].LDVAL = length; 				// set load value of timer
	PIT->CHANNEL[num].TCTRL |= (1 << 0);			// Enable timer (begin counting)
	PIT->CHANNEL[num].TCTRL |= (interrupts << 1);	// Enable timer interrupts		
}

void stopTimer(int num)
{	
	PIT->CHANNEL[num].TCTRL &= ~(1 << 0);	// Disable timer (stop counting)	
	resetTimer(num);
}

void resetTimer(int num)
{
	PIT->CHANNEL[num].TFLG |= (1 << 0); 	// Clear timer done flag
}
