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
	//STEP1 aktivieren
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPENABLE) |= (1<<1);
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
	//ADC starten
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1<<0);

	unsigned int LDR_Wert = 0;
	while (1)
	{
		//ADC Werte auslesen
		LDR_Wert = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(0));
		UARTprintf("LDR_Wert: %d\n", LDR_Wert);
		//delay_ms(500);
	}

	return 0;
}
