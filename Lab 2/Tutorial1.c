
#include <fsl_device_registers.h>

unsigned int counter = 0;

int main() {
	
	while (1) {
		counter++;
		if (counter > 0xFF)
		counter = 0;
	}
}
