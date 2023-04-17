// Fuegt hier die Header-Daten zu euren Funktionen ein

#include <stdlib.h>
#include "EGR_DCMotor.h"
#include <hw_types.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
#include <interrupt.h>
#include <uartStdio.h>
#include "EGR_Cape.h"
#include "Conf_mod.h"
#include "GPIO.h"



#ifndef MOTORSTEUERUNG_H_
#define MOTORSTEUERUNG_H_

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// GPIO Functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern void Motor_Speed();
extern void MotorKonfiguration();
extern void Motor1_Steuer(int Geschwindigkeit);
extern void Motor2_Steuer(int Geschwindigkeit);



#endif /* MOTORSTEUERUNG_H_ */
