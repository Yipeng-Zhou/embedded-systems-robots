/* EGR */
#include <uartStdio.h>
#include "IRSensors.h"

int main() {
	unsigned char IRResults = 0;

	I2C_IRSensors_Konfiguration();

	while (1)
	{
		IRResults = IRSensors();
		UARTprintf(BYTETOBINARYPATTERN, BYTETOBINARY(IRResults));
	}
	return 0;
}
