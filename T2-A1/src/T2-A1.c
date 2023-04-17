/* EGR */

#include <hw_types.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
#include <interrupt.h>
#include <uartStdio.h>
#include "EGR_Cape.h"
#include "Conf_mod.h"
#include "GPIO.h"
#include "delay_ms.h"

volatile unsigned int counter = 0;

//Interrupt Service Routine
void myGPIO2Isr() {
	//alles LEDs ausschalten
	EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN6_MODUL, GPIO_PORT2_PIN6, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN8_MODUL, GPIO_PORT2_PIN8, PIN_HIGH);

	counter = (!counter);

	UARTprintf("Interrupt!");

	//loeschen des Interrupt Flags
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN6);
}

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

	//Taster3 (PORT1, PIN6)
	//muxing to GPIO, Pullup
	PinMuxing(CONF_PORT1_PIN6, PULL_ENABLE, PULL_UP, GPIO_MODE);
	//pin als input setzen
	EGR_GPIODirSet(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6, GPIO_INPUT);

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

	//Led3 (PORT2, PIN6)
	//muxing to GPIO
	PinMuxing(CONF_PORT2_PIN6, PULL_DISABLE, PULL_UP, GPIO_MODE);
	//pin als output setzen
	EGR_GPIODirSet(GPIO_PORT2_PIN6_MODUL, GPIO_PORT2_PIN6, GPIO_OUTPUT);

	//Led4 (PORT2, PIN8)
	//muxing to GPIO
	PinMuxing(CONF_PORT2_PIN8, PULL_DISABLE, PULL_UP, GPIO_MODE);
	//pin als output setzen
	EGR_GPIODirSet(GPIO_PORT2_PIN8_MODUL, GPIO_PORT2_PIN8, GPIO_OUTPUT);

	//alles LEDs ausschalten
	EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN6_MODUL, GPIO_PORT2_PIN6, PIN_HIGH);
	EGR_PinWrite(GPIO_PORT2_PIN8_MODUL, GPIO_PORT2_PIN8, PIN_HIGH);

	//Interrupt
	//initialisiere ARM Interrupt Controller
	IntAINTCInit();
	//globale Interruptsteuerung aktivieren
	IntMasterIRQEnable();
	//lokale Interruptquelle aktivieren
	IntSystemEnable(SYS_INT_GPIOINT2A);
	//Registrierung der Funktion auf GPIO2-Interrupt A
	IntRegister(SYS_INT_GPIOINT2A, myGPIO2Isr);
	//Interrupt-Trigger setzen
	//Pin aufloesen
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1 << GPIO_PORT1_PIN6);
	//Ereignis an Pin aktivieren
	//Taster must zuerst als Pullup setzen, dann Ereignis aktivieren!
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN6);

	unsigned int j = 0;
	unsigned zeit = 20;

	while (1)
	{
		if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
			counter = (!counter);
			delay_ms(500);//prevent Taster changing the value of counter twitce during once press
		}

		if(counter == 1) {
			EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_LOW);
			for(j = 0; j < 25; j++) {
				if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
					counter = (!counter);
				}
				if(EGR_PinRead(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4) == PIN_LOW) {
					zeit = 10;
				}
				//if(EGR_PinRead(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6) == PIN_LOW) {
				//	break;
				//}
				delay_ms(zeit);
			}
			//if(j != 25) break;//jump out two loop
			EGR_PinWrite(GPIO_PORT2_PIN2_MODUL, GPIO_PORT2_PIN2, PIN_HIGH);
		}

		if(counter == 1) {
			EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_LOW);
			for(j = 0; j < 25; j++) {
				if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
					counter = (!counter);
				}
				if(EGR_PinRead(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4) == PIN_LOW) {
					zeit = 10;
				}
				//if(EGR_PinRead(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6) == PIN_LOW) {
				//	break;
				//}
				delay_ms(zeit);
			}
			//if(j != 25) break;//jump out two loop
			EGR_PinWrite(GPIO_PORT2_PIN4_MODUL, GPIO_PORT2_PIN4, PIN_HIGH);
		}

		if(counter == 1) {
			EGR_PinWrite(GPIO_PORT2_PIN6_MODUL, GPIO_PORT2_PIN6, PIN_LOW);
			for(j = 0; j < 25; j++) {
				if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
					counter = (!counter);
				}
				if(EGR_PinRead(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4) == PIN_LOW) {
					zeit = 10;
				}
				//if(EGR_PinRead(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6) == PIN_LOW) {
				//	break;
				//}
				delay_ms(zeit);
			}
			//if(j != 25) break;//jump out two loop
			EGR_PinWrite(GPIO_PORT2_PIN6_MODUL, GPIO_PORT2_PIN6, PIN_HIGH);
		}

		if(counter == 1) {
			EGR_PinWrite(GPIO_PORT2_PIN8_MODUL, GPIO_PORT2_PIN8, PIN_LOW);
			for(j = 0; j < 25; j++) {
				if(EGR_PinRead(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2) == PIN_LOW) {
					counter = (!counter);
				}
				if(EGR_PinRead(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4) == PIN_LOW) {
					zeit = 10;
				}
				//if(EGR_PinRead(GPIO_PORT1_PIN6_MODUL, GPIO_PORT1_PIN6) == PIN_LOW) {
				//	break;
				//}
				delay_ms(zeit);
			}
			//if(j != 25) break;//jump out two loop
			EGR_PinWrite(GPIO_PORT2_PIN8_MODUL, GPIO_PORT2_PIN8, PIN_HIGH);
		}
	}

	return 0;
}
