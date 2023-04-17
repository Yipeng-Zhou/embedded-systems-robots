/* EGR */

#include <hw_types.h>
#include <soc_AM335x.h>
#include <hw_tsc_adc_ss.h>
#include <uartStdio.h>
#include "delay_ms.h"

int main() {
	//Clock aktivieren
	HWREG(SOC_CM_WKUP_REGS + 0xbc) &= ~(0b11);
	HWREG(SOC_CM_WKUP_REGS + 0xbc) |= (1<<1);
	//Schreibschutz deaktivieren
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1<<2);
	//STEP1 and STEP2 aktivieren
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPENABLE) |= (1<<1);
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPENABLE) |= (1<<2);
	//STEP1 Konfiguration
	//  Modus waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) |= (0b01<<0);
	//  Filter einstellen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) |= (0b100<<2);
	//  Referenzspannung waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) |= (0b000<<12);
	//  Analogpins(AIN1) waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) |= (0b0001<<19);
	//  Speicher(FIFO0) waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) &= ~(1<<26);
	//STEP2 Konfiguration
	//  Modus waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= (0b01<<0);
	//  Filter einstellen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= (0b100<<2);
	//  Referenzspannung waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= (0b000<<12);
	//  Analogpins(AIN3) waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= (0b0011<<19);
	//  Speicher(FIFO1) waehlen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= (1<<26);
	//ADC starten
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1<<0);

	unsigned int Joy_Wert1 = 0;
	unsigned int Joy_Wert2 = 0;
	while (1)
	{
		//ADC Werte auslesen
		Joy_Wert1 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(0));
		Joy_Wert2 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(1));
		UARTprintf("Joy_Wert1: %d;\tJoy_Wert2: %d\n", Joy_Wert1,Joy_Wert2);
		delay_ms(500);
	}

	return 0;
}
