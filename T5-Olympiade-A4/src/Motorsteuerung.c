// Fuegt hier eure Funktionen zum Ansteuern der Motoren ein

#include "Motorsteuerung.h"

//Aufpassen!
volatile int speed = 0;

/*****************************************************************************
**                       FUNCTION DEFINITIONS
*****************************************************************************/

void MotorKonfiguration() {
	//initialisieren der EHRPWM-Module
	EHRPWMinitForDCMotor();
	//Pin Muxing der Motorpins im EHRPWM-Moduls
	PinMuxing(CONF_H_BRUECKE_MOTOR1_1, PULL_DISABLE, PULL_UP, 6);
	PinMuxing(CONF_H_BRUECKE_MOTOR1_2, PULL_DISABLE, PULL_UP, 6);
	PinMuxing(CONF_H_BRUECKE_MOTOR2_1, PULL_DISABLE, PULL_UP, 4);
	PinMuxing(CONF_H_BRUECKE_MOTOR2_2, PULL_DISABLE, PULL_UP, 4);
}

void Motor1_Steuer(int Geschwindigkeit) {
	//Forwrad
	if(Geschwindigkeit > 0) {
		configEHRPWM_A(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
		configEHRPWM_B(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
	}
	//Reverse
	else if(Geschwindigkeit < 0) {
		configEHRPWM_A(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
		configEHRPWM_B(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	}
	//Brake
	else {
		configEHRPWM_A(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_HIGH);
		configEHRPWM_B(SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_HIGH);
	}

	//Duty-Cycle
	EHRPWMsetDutyCycle(SOC_EPWM_1_REGS, abs(Geschwindigkeit));
}

void Motor2_Steuer(int Geschwindigkeit) {
	//Forwrad
	if(Geschwindigkeit > 0) {
		configEHRPWM_A(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
		configEHRPWM_B(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	}
	//Reverse
	else if(Geschwindigkeit < 0) {
		configEHRPWM_A(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
		configEHRPWM_B(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
	}
	//Brake
	else {
		configEHRPWM_A(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_HIGH);
		configEHRPWM_B(SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_HIGH);
	}

	//Duty-Cycle
	EHRPWMsetDutyCycle(SOC_EPWM_2_REGS, abs(Geschwindigkeit));
}

//Interrupt Service Routine
void myGPIO2Isr() {
	//Taster1(erhoehen)
	if((HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) & (1 << GPIO_PORT1_PIN2)) != 0) {
		speed = speed + 10;
		if(speed > 100) speed = 100;
		Motor1_Steuer(speed);
		Motor2_Steuer(speed);
		UARTprintf("Speed: %d\n", speed);
		//loeschen des Interrupt Flags
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN2);
	}

	//Taster2(Vermindern)
	if((HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) & (1 << GPIO_PORT1_PIN4)) != 0) {
		speed = speed - 10;
		if(speed < -100) speed = -100;
		Motor1_Steuer(speed);
		Motor2_Steuer(speed);
		UARTprintf("Speed: %d\n", speed);
		//loeschen des Interrupt Flags
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1 << GPIO_PORT1_PIN4);
	}

}

void Motor_Speed() {
	//Taster
	//muxing to GPIO, Pullup
	PinMuxing(CONF_PORT1_PIN2, PULL_ENABLE, PULL_UP, GPIO_MODE);
	PinMuxing(CONF_PORT1_PIN4, PULL_ENABLE, PULL_UP, GPIO_MODE);
	//pin als input setzen
	EGR_GPIODirSet(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2, GPIO_INPUT);
	EGR_GPIODirSet(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4, GPIO_INPUT);

	//Motor Konfiguration
	MotorKonfiguration();

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
	//Ereignis an Pin aktivieren
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN2);
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1 << GPIO_PORT1_PIN4);

}







