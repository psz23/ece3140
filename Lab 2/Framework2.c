#include <fsl_device_registers.h>

/*
     Main program: entry point
*/
int main (void)
{
	  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 3) */

	    /* your code goes here */
}


/* 
     PIT Interrupt Handler
*/

void PIT0_IRQHandler(void)
{
	  /* code goes here */
}
