#include <fsl_device_registers.h>

// timer clocks run at 20 MHz
#define LEDB_INTERVAL 0x01312D00 // ~1 second
#define LEDG_INTERVAL 0x001E8480 // ~0.1 seconds

#define LEDB_PIN 21
#define LEDG_PIN 26

void PIT0_IRQHandler(void);
void PIT1_IRQHandler(void);
void initLEDB(void); 
void setLEDB(int);
void initLEDG(void);
void setLEDG(int);
void initPITs(void);
void runTimer(int, int);
void stopTimer(int);
void resetTimer(int);  

volatile int LEDBStatus = 0;

int main(void)
{	
	initLEDB(); initLEDG(); initPITs(); 
	
	NVIC_EnableIRQ(PIT0_IRQn); 
	NVIC_EnableIRQ(PIT1_IRQn);
	
	runTimer(0, LEDB_INTERVAL);
	
	while(1) { }
}

void PIT0_IRQHandler(void)
{
	setLEDB(~LEDBStatus); 
	LEDBStatus = ~LEDBStatus;	
	resetTimer(0);	
	
	setLEDG(1);
	runTimer(1, LEDG_INTERVAL);
}

void PIT1_IRQHandler(void)
{
	setLEDG(0); 
	stopTimer(1);
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

void runTimer(int num, int length)
{
	PIT->CHANNEL[num].LDVAL = length; 		// set load value of timer
	PIT->CHANNEL[num].TCTRL |= (1 << 0);	// Enable timer (begin counting)
	PIT->CHANNEL[num].TCTRL |= (1 << 1);	// Enable timer interrupts
	PIT->CHANNEL[num].TFLG |= (1 << 0);		// Clear timer done flag		
}

void stopTimer(int num)
{
	PIT->CHANNEL[num].TCTRL &= ~(1 << 0);	// Disable timer
}

void resetTimer(int num)
{
	PIT->CHANNEL[num].TFLG |= (1 << 0);
}
