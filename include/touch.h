/**
 * touch.h
 *
 *  Created on: 29.05.2009
 *      Author: sebastian
 */

#ifndef TOUCH_H_
#define TOUCH_H_ TOUCH_H_

#include "adc.h"


#define TOUCH_PORT 	PORTA
#define TOUCH_PIN	PINA
#define TOUCH_DDR  	DDRA

#define TOUCH_X1 PA1
#define TOUCH_X2 PA3
#define TOUCH_Y1 PA5
#define TOUCH_Y2 PA7

#define TOUCH_PRESSED_LEVEL 1000

uint8_t touch_is_pressed();

int16_t touch_readX();
int16_t touch_readY() ;

#endif /* TOUCH_H_ */
