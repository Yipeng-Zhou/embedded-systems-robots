/* EGR */

#include <hw_types.h>
#include <soc_AM335x.h>
#include <hw_tsc_adc_ss.h>
#include <uartStdio.h>
#include "delay_ms.h"
#include "Motorsteuerung.h"

#define MAXJOYWERT 4000
#define MINJOYWERT 0
#define MIDDLEJOYWERT 2000


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

	//Motor Konfiguration
	MotorKonfiguration();

	int Joy_Wert1 = 0;
	int Joy_Wert2 = 0;
	int speed_1 = 0;
	int speed_2 = 0;
	while (1)
	{
		//ADC Werte auslesen
		Joy_Wert1 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(0));
		Joy_Wert2 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(1));
		if(Joy_Wert1 > MAXJOYWERT) Joy_Wert1 = MAXJOYWERT;
		if(Joy_Wert1 < MINJOYWERT) Joy_Wert1 = MINJOYWERT;
		if(Joy_Wert2 > MAXJOYWERT) Joy_Wert2 = MAXJOYWERT;
		if(Joy_Wert2 < MINJOYWERT) Joy_Wert2 = MINJOYWERT;
		speed_1 = ((Joy_Wert1 - MIDDLEJOYWERT) * 100) / (MAXJOYWERT - MIDDLEJOYWERT);
		speed_2 = ((Joy_Wert2 - MIDDLEJOYWERT) * 100) / (MAXJOYWERT - MIDDLEJOYWERT);
		Motor1_Steuer(speed_1);
		Motor2_Steuer(speed_2);

		//UARTprintf("Joy_Wert1: %d;\tJoy_Wert2: %d\n", Joy_Wert1, Joy_Wert2);
		UARTprintf("Speed_1: %d;\tSpeed_2: %d\n", speed_1, speed_2);
		//delay_ms(500);
	}

	return 0;
}
