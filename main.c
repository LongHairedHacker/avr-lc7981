/*
 * main.c
 *
 *  Created on: 01.05.2009
 *      Author: sebastian
 */

#include  <avr/io.h>
#include  <stdlib.h>

#include "include/lc7981.h"
#include "include/adc.h"
//#include "button.h"
#include "12x16_horizontal_LSB_1.h"
//#include "8x8_horizontal_LSB_1.h"
#include "include/touch.h"

#include "writing_demo.h"




int main() {


	lcd_init(LCD_GRAPHIC);


		lcd_plot_text(5,5,"Hello",16,16,font_12x16);
		lcd_plot_pgmtext(50,22,PSTR("World"),16,16,font_12x16);


		while(!touch_is_pressed());

		writing_demo();

	return 0;

}
