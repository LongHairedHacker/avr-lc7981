/**
 * writing_demo.c
 *
 *  Created on: 29.05.2009
 *      Author: sebastian
 */


#include "writing_demo.h"



void writing_demo() {
	uint16_t x,y;

	lcd_clear();

	while(1) {

		if(touch_is_pressed()) {

			x = touch_readX();
			y = touch_readY();

			lcd_plot_pixel(x,y+1,PIXEL_ON);
			lcd_plot_pixel(x,y,PIXEL_ON);
			lcd_plot_pixel(x+1,y+1,PIXEL_ON);
			lcd_plot_pixel(x+1,y,PIXEL_ON);


		}
			_delay_ms(1);

	}

}
