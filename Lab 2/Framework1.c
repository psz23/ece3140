#include <fsl_device_registers.h>

int main (void)
{
	PIT->CHANNEL[0].LDVAL = 0x30000; // set load value of zeroth PIT
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // ENABLE CLOCK TO PIT MODULE
}
