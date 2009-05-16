/*
 * lc7981.c
 *
 *  Created on: 01.05.2009
 *      Author: sebastian
 */

#include "lc7981.h"

uint8_t lcd_mode, 		//!< Currently used mode, can be text or graphic.
		lcd_curline;	//!< Current line. Used for scrolling in text mode.

/**
 * lcd_init set the to default values.
 *
 * @param mode LCD_TEXT for text mode and LCD_GRAPHIC for graphic mode
 */
void lcd_init(uint8_t mode) {

	LCD_DATA_DDR = 0xFF;
	LCD_DATA = 0;
	LCD_CRTL_DDR = 0xFF;
	//|= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN) | (1 << LCD_CS) | (1 << LCD_RST);
	LCD_CTRL = 0;


	lcd_en_low();
	lcd_rs_low();
	lcd_rw_low();

	if(mode == LCD_TEXT) {

		lcd_mode = LCD_TEXT;

		lcd_write_command(0x00,0x3C);
		lcd_write_command(0x01,0x75);
		lcd_write_command(0x02,LCD_TEXT_COLUMNS - 1);
		lcd_write_command(0x03,0x4F);
		lcd_write_command(0x04,0x07);
		lcd_write_command(0x08,0x00);
		lcd_write_command(0x09,0x00);
		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);

	}
	else {

		lcd_mode = LCD_GRAPHIC;

		lcd_write_command(0x00,0x32);
		lcd_write_command(0x01,0x07);
		lcd_write_command(0x02,(LCD_GRAPHIC_WIDTH / 8)-1);
		lcd_write_command(0x03,0x4F);
		lcd_write_command(0x08,0x00);
		lcd_write_command(0x09,0x00);
		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);
	}

	lcd_clear();
}

/**
 *  Writes a command and a data byte to the lcd.
 *
 *  @param cmd the command byte
 *  @param data the data that is going to be written after the command
 */
inline void lcd_write_command(uint8_t cmd, uint8_t data) {
	_delay_us(30);
	lcd_rw_low();
	lcd_rs_high();
	LCD_DATA = cmd;
	_delay_us(1);
	lcd_strobe();

	lcd_rs_low();
	LCD_DATA = data;
	_delay_us(1);
	lcd_strobe();

}

/**
 * Reads a byte from the display memory.
 * lcd_gotoxy can be used to set the location.
 * Important : lcd_gotoxy doesn't work in graphics mode yet.
 *
 * @return the byte which has been read
 * @see lcd_gotoxy
 */
inline uint8_t lcd_read_byte() {
uint8_t i,data;

	for(i = 0; i < 2; i++) {
		_delay_us(30);
		lcd_rw_low();
		lcd_rs_high();
		LCD_DATA = 0x0D;
		_delay_us(1);
		lcd_en_high();

		LCD_DATA_DDR = 0x00;
		lcd_rs_low();
		lcd_rw_high();
		_delay_us(1);
		data = (uint8_t) PIND;
		lcd_en_low();
		LCD_DATA_DDR = 0xFF;
	}
	return data;
}


/**
 * Generates the strobe signal for writing data.
 * This function is meant for internal usage only.
 */
inline void lcd_strobe() {
	lcd_en_high();
	_delay_us(1);
	lcd_en_low();

}

/**
 * Clears the display by setting the whole memory to 0.
 */
inline void lcd_clear() {
uint16_t i;

	if(lcd_mode == LCD_TEXT) {

		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);

		for(i = 1; i <= LCD_TEXT_LINES * LCD_TEXT_COLUMNS; i++)
			lcd_write_command(0x0C,' ');

		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);

		lcd_curline = 0;
	}
	else {

		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);

		for(i = 0; i < ((LCD_GRAPHIC_WIDTH * LCD_GRAPHIC_HEIGHT)/8); i++)
			lcd_write_command(0x0C,0x00);

		lcd_write_command(0x0A,0x00);
		lcd_write_command(0x0B,0x00);
	}


}



void lcd_write_text(char *txt) {
uint8_t c,tmp,x,y;


	if(lcd_mode == LCD_TEXT) {
		c = 0;
		while(!(*txt == 0)) {

			if(*txt == '\n' || c == LCD_TEXT_COLUMNS) {
				if(lcd_curline < LCD_TEXT_LINES - 1) {
					lcd_curline++;
					c = 0;
					lcd_gotoxy(0,lcd_curline);
				}
				else {
					for(y = 1; y < LCD_TEXT_LINES; y++ ) {
						for(x = 0; x < LCD_TEXT_COLUMNS; x++) {
							lcd_gotoxy(x,y);
							tmp = lcd_read_byte();
							lcd_gotoxy(x,y-1);
							lcd_write_command(0x0C,tmp);
						}
					}
					for(x = 0; x < LCD_TEXT_COLUMNS; x++) {
						lcd_write_command(0x0C,' ');
					}
					lcd_gotoxy(0,LCD_TEXT_LINES-1);
					c = 0;
				}
			}

			if(*txt != '\n') {
				lcd_write_command(0x0C,*txt);
				c++;
			}
			txt++;
		}

	}
}

/**
 * Set the display cursor to a specific location.
 * Right now this does only work in text mode,
 * since there's no need for it now in graphics mode.	\n
 * If the x and y values are to big, the function does nothing.
 *
 * @param x the horizontal cursorposition (starts at 0)
 * @param y the vertical cursorposition (starts at 0)
 *
 */
inline void lcd_gotoxy(uint8_t x, uint8_t y) {
uint16_t pos;

	if(lcd_mode == LCD_TEXT) {
		if((x <= LCD_TEXT_COLUMNS - 1)  && (y <= LCD_TEXT_LINES - 1)) {
			pos = (y * LCD_TEXT_COLUMNS) + x;

			lcd_write_command(0x0A,(uint8_t) pos );
			lcd_write_command(0x0B,(uint8_t) (pos  >> 8));
			lcd_curline = y;
		}
	}
}

/**
 * set or unset a pixel at the specified location
 *
 * @param x horizontal position of the pixel (starts at 0)
 * @param y horizontal position of the pixel (starts at 0)
 * @param set can be PIXEL_ON or PIXEL_OFF
 */
void lcd_plot_pixel(uint8_t x, uint8_t y, uint8_t set) {
uint16_t pos;
uint8_t xr;

	if((x <= LCD_GRAPHIC_WIDTH - 1) && (y <= LCD_GRAPHIC_HEIGHT - 1)) {
		pos = y * (LCD_GRAPHIC_WIDTH / 8)  + x / 8;
		xr = x % 8;
		lcd_write_command(0x0A,(uint8_t) pos );
		lcd_write_command(0x0B,(uint8_t) (pos  >> 8));

		if(set == PIXEL_ON) {
			lcd_write_command(0x0F,xr);
		}
		else {
			lcd_write_command(0x0E,xr);
		}

	}
}
/**
 * This function will plot a bitmap with the upper left corner to the given coordinates.
 */
void lcd_plot_bitmap(uint8_t x_off, uint8_t y_off, const uint8_t *bitmap, uint8_t w, uint8_t h) {
uint8_t x,y,cur,curs,sr,dr;
uint16_t pos;

	if((x_off <= LCD_GRAPHIC_WIDTH - 1) && (y_off <= LCD_GRAPHIC_HEIGHT - 1)
		&& (x_off + w <= LCD_GRAPHIC_WIDTH - 1) && (y_off + h <= LCD_GRAPHIC_HEIGHT - 1)) {
		for(y = y_off; y < y_off + h; y++) {
			cur = 0;
			for(x = x_off - (x_off % 8); x_off + w > x; x++) {
				dr = x % 8;

				if(dr == 0) {
					pos = y * (LCD_GRAPHIC_WIDTH / 8)  + x / 8;
					lcd_write_command(0x0A,(uint8_t) pos );
					lcd_write_command(0x0B,(uint8_t) (pos  >> 8));
					cur = lcd_read_byte();
				}

				if((x - x_off) >= 0) {
					sr = (x - x_off) % 8;
					if(sr == 0) {
						curs = pgm_read_byte(bitmap++);
					}

					cur = cur | ((( curs & (1 << sr)) >> sr) << dr);
				}

				if(dr == 7) {
					pos = y * (LCD_GRAPHIC_WIDTH / 8)  + x / 8;
					lcd_write_command(0x0A,(uint8_t) pos );
					lcd_write_command(0x0B,(uint8_t) (pos  >> 8));
					lcd_write_command(0x0C,cur);
				}
			}
			if(dr != 7) {
				pos = y * (LCD_GRAPHIC_WIDTH / 8)  + x / 8;
				lcd_write_command(0x0A,(uint8_t) pos );
				lcd_write_command(0x0B,(uint8_t) (pos  >> 8));
				lcd_write_command(0x0C,cur);
			}

		}

	}
}
