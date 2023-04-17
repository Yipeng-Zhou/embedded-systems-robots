/* EGR */

#include <hw_types.h>
#include "EGR_Cape.h"
#include "Conf_mod.h"
#include "GPIO.h"

int main() {
	//Taster1 (PORT1, PIN2)
	//muxing to GPIO, Pullup
	PinMuxing(CONF_PORT1_PIN2, PULL_ENABLE, PULL_UP, GPIO_MODE);
	//pin als input setzen
	EGR_GPIODirSet(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2, GPIO_INPUT);

	//Taster2 (PORT1, PIN4)
	//muxing to GPIO, Pullup
	PinMuxing(CONF_PORT1_PIN4, PULL_ENABLE, PULL_UP, GPIO_MODE);
	//pin als input setzen
	EGR_GPIODirSet(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4, GPIO_INPUT);

	//Led1 (PORT2, PIN2)
	//muxing to GPIO
	PinMuxing(CONF_PORT2_PIN2, PULL_DISABLE, PULL_UP, GPIO_MODE);
	//pin als output setzen
	EGR_GPIODirSet(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, GPIO_OUTPUT);

	//Led2 (PORT2, PIN4)
	//muxing to GPIO
	PinMuxing(CONF_PORT2_PIN4, PULL_DISABLE, PULL_UP, GPIO_MODE);
	//pin als output setzen
	EGR_GPIODirSet(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, GPIO_OUTPUT);


	while (1)
	{
		//conrol LED1
		if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
			EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_LOW);
		}
		else {
			EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_HIGH);
		}

		//conrol LED2
		if(EGR_PinRead(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4) == PIN_LOW) {
			EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_LOW);
		}
		else {
			EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_HIGH);
		}
	}
	return 0;
}
