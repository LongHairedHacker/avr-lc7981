/*
 * main.c
 *
 *  Created on: 01.05.2009
 *      Author: sebastian
 */

#include  <avr/io.h>
#include  <stdlib.h>

#include "include/lc7981.h"
#include "button.h"
#include "12x16_horizontal_LSB_1.h"
#include "8x8_horizontal_LSB_1.h"




int main() {
	uint8_t x,y;
	uint16_t pos;
	const uint8_t *letter;

	lcd_init(LCD_GRAPHIC);




	//while(1) {


		lcd_plot_text(5,5,"Hello",16,16,font_12x16);
		lcd_plot_text(50,22,"World",16,16,font_12x16);
		lcd_plot_text(5,40,"Hallo Welt !",8,8,font_8x8);



	/*for(x = 1; x < 10; x++) {
	_delay_ms(500);
	}
	lcd_clear();


	for(x = 31; x > 0; x--) {
		lcd_plot_char(x,12,'A',12,16,font_12x16);

	_delay_ms(500);
	lcd_clear();
	}
	}

*/


	/*

	while(1) {
	for(y = 1; y <= 32; y++) {
	lcd_plot_bitmap(16+y,16,bitmap,32,32);
	_delay_ms(500);
	lcd_clear();
	}

	for(y = 31; y > 0; y--) {
	lcd_plot_bitmap(16+y,16,bitmap,32,32);
	_delay_ms(500);
	lcd_clear();
	}
	}
*/


	/*draw_penguin(); */


	/*for(i = 0; i <= 8; i++) {
		lcd_gotoxy(0,i);
		lcd_write_text("Zeile : ");
		lcd_write_text(itoa(i,tmp,10));
		lcd_write_text("\n");
		_delay_ms(100);
	}
	_delay_ms(100);



	lcd_write_text("123456789012345678901234567890\n");
	lcd_write_text("Letzte Zeile"); */

	return 0;

}
