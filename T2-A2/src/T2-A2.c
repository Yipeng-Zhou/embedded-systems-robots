/* EGR */

#include <math.h>
#include <hw_types.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
#include <hw_dmtimer.h>
#include <interrupt.h>
#include <uartStdio.h>
#include "EGR_Cape.h"
#include "Conf_mod.h"
#include "GPIO.h"

float period = 20;
volatile float steuersignal = 0;

//Interrupt Service Routine
void myGPIO2Isr() {
	//Taster1
	if((HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) & (1 << GPIO_PORT1_PIN2)) != 0) {
		steuersignal = 1;
		UARTprintf("Taster1!");
		//loeschen des Interrupt Flags
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN2);
	}

	//Taster2
	if((HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) & (1 << GPIO_PORT1_PIN4)) != 0) {
		steuersignal = 2;
		UARTprintf("Taster2!");
		//loeschen des Interrupt Flags
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN4);
	}

	//Taster3
	if((HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) & (1 << GPIO_PORT1_PIN6)) != 0) {
		steuersignal = 1.5;
		UARTprintf("Taster3!");
		//loeschen des Interrupt Flags
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN6);
	}

	//TIMER Konfiguration
	unsigned int timer_startwert = pow(2,32) - period*24000;
	unsigned int timer_comparewert = pow(2,32) - (period - steuersignal)*24000;
	//Auto-Reload Modus aktivieren
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCLR) |= (1 << 1);
	//disable Prescaler
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCLR) &= ~(1 << 5);
	//Compare Modus aktivieren
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCLR) |= (1 << 6);
	//timer start-wert setzen
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCRR) = timer_startwert;
	//timer load-wert setzen
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TLDR) = timer_startwert;
	//timer compare-wert setzen
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TMAR) = timer_comparewert;
	//timer starten
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCLR) |= (1 << 0);

}

void myTimer2Isr() {
	//overflow interrupt
	if((HWREG(SOC_DMTIMER_2_REGS + DMTIMER_IRQSTATUS) & (1 << 1)) != 0) {
		//pin-wert auf HIGH setzen
		EGR_PinWrite(GPIO_PORT3_PIN2_MODUL, GPIO_PORT3_PIN2, PIN_HIGH);

		//UARTprintf("Overflow!");

		//loeschen des Timer-Interrupt Flags
		HWREG(SOC_DMTIMER_2_REGS + DMTIMER_IRQSTATUS) |= (1 << 1);
	}
	//compare match interrupt
	else if((HWREG(SOC_DMTIMER_2_REGS + DMTIMER_IRQSTATUS) & (1 << 0)) != 0) {
		//pin-wert auf LOW setzen
		EGR_PinWrite(GPIO_PORT3_PIN2_MODUL, GPIO_PORT3_PIN2, PIN_LOW);

		//UARTprintf("Compare Match!");

		//loeschen des Timer-Interrupt Flags
		HWREG(SOC_DMTIMER_2_REGS + DMTIMER_IRQSTATUS) |= (1 << 0);
	}
}

int main() {
	//Servo-Antrieb (PORT3, PIN2)
	//muxing to GPIO
	PinMuxing(CONF_PORT3_PIN2, PULL_DISABLE, PULL_UP, GPIO_MODE);
	//pin als output setzen
	EGR_GPIODirSet(GPIO_PORT3_PIN2_MODUL, GPIO_PORT3_PIN2, GPIO_OUTPUT);

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

	//GPIO-Interrupt konfiguration
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
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1 << GPIO_PORT1_PIN2);
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1 << GPIO_PORT1_PIN4);
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1 << GPIO_PORT1_PIN6);
	//Ereignis an Pin aktivieren
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN2);
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN4);
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN6);
	//Prioritaet einstellen
	IntPrioritySet(SYS_INT_GPIOINT2A, 0, AINTC_HOSTINT_ROUTE_IRQ);

	//TIMER-Interrupt Konfiguration
	//initialisiere ARM Interrupt Controller
	//IntAINTCInit();
	//globale interruptsteuerung aktivieren
	//IntMasterIRQEnable();
	//lokale interruptquelle aktivieren
	IntSystemEnable(SYS_INT_TINT2);
	//registrierung der Funktion auf TIMER2-Interrupt
	IntRegister(SYS_INT_TINT2, myTimer2Isr);
	//Interrupt-Trigger(Compare-Match Interrupt and Overflow Interrupt) setzen
	HWREG(SOC_DMTIMER_2_REGS + DMTIMER_IRQENABLE_SET) |= ((1 << 0) | (1<< 1));
	//Prioritaet einstellen
	IntPrioritySet(SYS_INT_TINT2, 1, AINTC_HOSTINT_ROUTE_IRQ);

	//pin-wert auf HIGH setzen beim timer_startwert
	EGR_PinWrite(GPIO_PORT3_PIN2_MODUL, GPIO_PORT3_PIN2, PIN_HIGH);

	return 0;
}
