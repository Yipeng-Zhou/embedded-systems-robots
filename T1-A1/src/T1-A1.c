/* EGR */

#include <hw_types.h>

int main() {
	//LED1 (LCD_DATA6, gpio2_12)
	//muxing to GPIO
	//control module: 0x44E10000; LCD_DATA6: 0x8B8; GPIO: 0~2
	HWREG(0x44E10000 + 0x8B8) &= ~((1<<2)|(1<<1)|(1<<0));
	HWREG(0x44E10000 + 0x8B8) |= ((1<<2)|(1<<1)|(1<<0));

	//pin als output setzen
	//GPIO2: 0x481AC000; GPIO_OE: 0x134
	HWREG(0x481AC000 + 0x134) &= ~(1<<12);

	//output of pin als 0 setzen
	//GPIO2: 0x481AC000; GPIO_DATAOUT: 0x13C
	HWREG(0x481AC000 + 0x13C) &= ~(1<<12);
	while (1)
	{
		/* Place main loop code here */
	}
	return 0;
}
