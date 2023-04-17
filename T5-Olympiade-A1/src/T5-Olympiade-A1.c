/* EGR */

#include <hw_types.h>
#include <uartStdio.h>
#include "IRSensors.h"
#include "Motorsteuerung.h"
#include "delay_ms.h"

int main() {
	unsigned char IRResults = 0;

	//IRSensors Konfiguration
	I2C_IRSensors_Konfiguration();
	//Motor Konfiguration
	MotorKonfiguration();

	//Taster3 (PORT1, PIN6)
	//muxing to GPIO, Pullup
	PinMuxing(CONF_PORT1_PIN6, PULL_ENABLE, PULL_UP, GPIO_MODE);
	//pin als input setzen
	EGR_GPIODirSet(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6, GPIO_INPUT);

	while (1) {
		if(EGR_PinRead(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6) == PIN_LOW) {

			delay_ms(5000);

			while (1) {
				IRResults = IRSensors();
				UARTprintf(BYTETOBINARYPATTERN, BYTETOBINARY(IRResults));

				if((((IRResults & (1<<6)) >> 6) == 0) && ((IRResults & (1<<5)) >> 5) == 1 && (((IRResults & (1<<4)) >> 4) == 1)) {
					Motor1_Steuer(80);
					Motor2_Steuer(100);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 0) && ((IRResults & (1<<5)) >> 5) == 0 && (((IRResults & (1<<4)) >> 4) == 1)) {
					Motor1_Steuer(100);
					Motor2_Steuer(50);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 0) && ((IRResults & (1<<5)) >> 5) == 0 && (((IRResults & (1<<4)) >> 4) == 0)){
					Motor1_Steuer(100);
					Motor2_Steuer(0);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 1) && ((IRResults & (1<<5)) >> 5) == 1 && (((IRResults & (1<<4)) >> 4) == 1)){
					Motor1_Steuer(0);
					Motor2_Steuer(100);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 1) && ((IRResults & (1<<5)) >> 5) == 0 && (((IRResults & (1<<4)) >> 4) == 1)){
					Motor1_Steuer(100);
					Motor2_Steuer(50);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 1) && ((IRResults & (1<<5)) >> 5) == 0 && (((IRResults & (1<<4)) >> 4) == 0)){
					Motor1_Steuer(100);
					Motor2_Steuer(0);
					delay_ms(10);
				}
				else if((((IRResults & (1<<6)) >> 6) == 1) && ((IRResults & (1<<5)) >> 5) == 1 && (((IRResults & (1<<4)) >> 4) == 0)){
					Motor1_Steuer(100);
					Motor2_Steuer(0);
					delay_ms(10);
				}
			}
		}
	}

	return 0;
}
