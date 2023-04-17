// Verlinkt
#include "IRSensors.h"

//slave adress and write/read
unsigned int sla_address = 0b0111000;
//Menge der Byte to send/receive
unsigned char count_1 = 1;
unsigned char count_2 = 2;
//Stop Condition
char stp_0 = 0;
char stp_1 = 1;
//Data to send/receive
unsigned char data_send[50];
unsigned char data_receive[50];

void I2C_IRSensors_Konfiguration() {
	//Initialisieren I2C
	initi2c();

	//Konfiguration der Pin auf dem Port-Expander als Input oder Outport
	data_send[0] = 3;
	data_send[1] = 0b01111111;
	writetoi2c(sla_address, data_send, count_2, stp_1);
}

unsigned char IRSensors() {
	//IR-LED einschalten
	data_send[0] = 1;
	data_send[1] = 0b10000000;
	writetoi2c(sla_address, data_send, count_2, stp_1);

	//IR-LED auslesen
	data_send[0] = 0;
	writetoi2c(sla_address, data_send, count_1, stp_0);
	readfromi2c(sla_address, data_receive, count_1, stp_1);

	//IR-LED ausschalten
	data_send[0] = 1;
	data_send[1] = 0b00000000;
	writetoi2c(sla_address, data_send, count_2, stp_1);

	delay_ms(1);

	return data_receive[0];

}
