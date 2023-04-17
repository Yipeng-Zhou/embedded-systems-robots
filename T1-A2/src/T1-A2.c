/* EGR */

#include <hw_types.h>

int main() {
	//Taster1 (LCD_DATA6, gpio2_12)
	//muxing to GPIO
	//control module: 0x44E10000; LCD_DATA6: 8B8; GPIO: 0~2
	HWREG(0x44E10000 + 0x8B8) &= ~((1<<2)|(1<<1)|(1<<0));
	HWREG(0x44E10000 + 0x8B8) |= ((1<<2)|(1<<1)|(1<<0));
	//pullup enable: 3
	HWREG(0x44E10000 + 0x8B8) &= ~(1<<3);
	//pullup setzen: 4
	HWREG(0x44E10000 + 0x8B8) &= ~(1<<4);
	HWREG(0x44E10000 + 0x8B8) |= (1<<4);
	//pin als input setzen
	//GPIO2: 0x481AC000; GPIO_OE: 0x134
	HWREG(0x481AC000 + 0x134) |= (1<<12);

	//Taster2 (LCD_DATA4, gpio2_10)
	//muxing to GPIO
	//control module: 0x44E10000; LCD_DATA4: 8B0; GPIO: 0~2
	HWREG(0x44E10000 + 0x8B0) &= ~((1<<2)|(1<<1)|(1<<0));
	HWREG(0x44E10000 + 0x8B0) |= ((1<<2)|(1<<1)|(1<<0));
	//pullup enable: 3
	HWREG(0x44E10000 + 0x8B0) &= ~(1<<3);
	//pullup setzen: 4
	HWREG(0x44E10000 + 0x8B0) &= ~(1<<4);
	HWREG(0x44E10000 + 0x8B0) |= (1<<4);
	//pin als input setzen
	//GPIO2: 0x481AC000; GPIO_OE: 0x134
	HWREG(0x481AC000 + 0x134) |= (1<<10);

	//Led1 (LCD_DATA14, gpio0_10)
	//muxing to GPIO
	//control module: 0x44E10000; LCD_DATA14: 0x8D8; GPIO: 0~2
	HWREG(0x44E10000 + 0x8D8) &= ~((1<<2)|(1<<1)|(1<<0));
	HWREG(0x44E10000 + 0x8D8) |= ((1<<2)|(1<<1)|(1<<0));
	//pin als output setzen
	//GPIO0: 0x44E07000; GPIO_OE: 0x134
	HWREG(0x44E07000 + 0x134) &= ~(1<<10);

	//Led2 (LCD_DATA13, gpio0_9)
	//muxing to GPIO
	//control module: 0x44E10000; LCD_DATA13: 0x8D4; GPIO: 0~2
	HWREG(0x44E10000 + 0x8D4) &= ~((1<<2)|(1<<1)|(1<<0));
	HWREG(0x44E10000 + 0x8D4) |= ((1<<2)|(1<<1)|(1<<0));
	//pin als output setzen
	//GPIO0: 0x44E07000; GPIO_OE: 0x134
	HWREG(0x44E07000 + 0x134) &= ~(1<<9);


	while (1)
	{
		//conrol LED1
		//GPIO2: 0x481AC000; GPIO_DATAIN: 0x138
		if ((HWREG(0x481AC000 + 0x138) & (1<<12)) == 0) {
			//output of pin of LED1 als 0 setzen
			//GPIO0: 0x44E07000; GPIO_DATAOUT: 0x13C
			HWREG(0x44E07000 + 0x13C) &= ~(1<<10);
		}
		else {
			//output of pin of LED1 als 1 setzen
			//GPIO0: 0x44E07000; GPIO_DATAOUT: 0x13C
			HWREG(0x44E07000 + 0x13C) |= (1<<10);
		}

		//conrol LED2
		//GPIO2: 0x481AC000; GPIO_DATAIN: 0x138
		if ((HWREG(0x481AC000 + 0x138) & (1<<10)) == 0) {
			//output of pin of LED2 als 0 setzen
			//GPIO0: 0x44E07000; GPIO_DATAOUT: 0x13C
			HWREG(0x44E07000 + 0x13C) &= ~(1<<9);
		}
		else {
			//output of pin of LED2 als 1 setzen
			//GPIO0: 0x44E07000; GPIO_DATAOUT: 0x13C
			HWREG(0x44E07000 + 0x13C) |= (1<<9);
		}
	}
	return 0;
}
