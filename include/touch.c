/*
 * touch.c
 *
 *  Created on: 29.05.2009
 *      Author: sebastian
 */

#include "touch.h"



/**
 * This functions set 5V to X1 and GND to Y1,
 * the voltage at X2 is measured via the ADC.
 * If the ADC-Value is less then TOUCH_PRESSED_LEVEL,
 * the function will return 1.
 * \return 1 if pressed else 0
 */
uint8_t touch_is_pressed() {
	TOUCH_DDR |= (1 << TOUCH_X1);
	TOUCH_DDR |= (1 << TOUCH_Y1);
	TOUCH_DDR &= ~(1 << TOUCH_X2);

	TOUCH_PORT &= ~(1 << TOUCH_Y1);
	TOUCH_PORT |= (1 << TOUCH_X1);

	if(readADC(TOUCH_X2) < TOUCH_PRESSED_LEVEL) {
		return 1;
	}
	else {
		return 0;
	}

}
/**
 *
 *
 */
int16_t touch_readX() {
	int16_t pos;

	if(touch_is_pressed()) {

		TOUCH_DDR |= (1 << TOUCH_X1);
		TOUCH_DDR |= (1 << TOUCH_X2);
		TOUCH_DDR &= ~(1 << TOUCH_Y1);
		TOUCH_DDR &= ~(1 << TOUCH_Y2);

		TOUCH_PORT &= ~(1 << TOUCH_X1);
		TOUCH_PORT |= (1 << TOUCH_X2);

		_delay_us(10);

		pos = (readADC(TOUCH_Y1) - 263) * 16 / 53;


		return pos;


	}
	else  {
		return -1;
	}

}


int16_t touch_readY() {
	int16_t pos;

	if(touch_is_pressed()) {

		TOUCH_DDR |= (1 << TOUCH_Y1);
		TOUCH_DDR |= (1 << TOUCH_Y2);
		TOUCH_DDR &= ~(1 << TOUCH_X1);
		TOUCH_DDR &= ~(1 << TOUCH_X2);

		TOUCH_PORT |= (1 << TOUCH_Y1);
		TOUCH_PORT &= ~(1 << TOUCH_Y2);

		_delay_us(10);

		pos = (readADC(TOUCH_X1) - 330) * 8 / 40;


		return pos;


	}
	else  {
		return -1;
	}

}
