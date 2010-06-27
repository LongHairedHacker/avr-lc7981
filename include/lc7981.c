/*
 * lc7981.c
 *
 * Version 0.7 beta
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Sebastians AVR Library for lc7981.
 *
 * The Initial Developer of the Original Code is Sebastian Schumb (webmaster@sebastians-site.de).
 *
 * Contributor(s): None so far.
 *
 * Any version below 0.6 is still licensed under GPL.
 * The GPLed versions will neither be supported, nor developed any further.
 *
 *
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
	LCD_CRTL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);
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
 * Clears the display by setting the whole memory to 0.
 */
void lcd_clear() {
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


/**
 * Outputs text in at the current cursor position.
 * If a \\n occurs in the last line of the display,
 * all the other line will be 'scrolled' up.
 * This function is textmode only.
 * The the second line becomes the first, the third the second and so on ...
 * @param txt the text
 */
void lcd_write_text(char *txt) {
uint8_t c,tmp,x,y;
uint16_t pos;


	if(lcd_mode == LCD_TEXT) {
		c = 0; //variable to count the chars in the current line
		while(!(*txt == 0)) { //loop through the string

			if(*txt == '\n' || c == LCD_TEXT_COLUMNS) { //linebreak if //n or if a line is too long
				if(lcd_curline < LCD_TEXT_LINES - 1) { //next line
					lcd_curline++;
					c = 0;
					lcd_gotoxy(0,lcd_curline);
				}
				else { //scroll up
					for(y = 1; y < LCD_TEXT_LINES; y++ ) {
						for(x = 0; x < LCD_TEXT_COLUMNS; x++) {
							pos = (y * LCD_TEXT_COLUMNS) + x;
							tmp = lcd_read_byte(pos);
							lcd_gotoxy(x,y-1);
							lcd_write_command(0x0C,tmp);
						}
					}
					for(x = 0; x < LCD_TEXT_COLUMNS; x++) { //free the last line
						lcd_write_command(0x0C,' ');
					}
					lcd_gotoxy(0,LCD_TEXT_LINES-1);
					c = 0;
				}
			}

			if(*txt != '\n') { //write the character
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
void lcd_gotoxy(uint8_t x, uint8_t y) {
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
 * This function will plot a bitmap with the upper left corner to the given coordinates.\n
 * If the bitmap doesn't fit on the display at this location, nothing is drawn at all.\n
 * Unset pixels are treated as transparent \n
 * This function is by far the most tricky piece of code in this project,\n
 * don't worry if you don't understand it at first sight. \n
 * Actually it just loops bitwise through the bitmap and uses some \n
 * nice bit shifting magic to get it anywhere you want.
 *
 * @param x horizontal position of the bitmap
 * @param y vertical position of the bitmap
 * @param bitmap pointer to the bitmap in pgmspace
 * @param w width of the bitmap
 * @param h height of the bitmap
 *
 * This function is dedicated to Greta, one of the most important persons in my life so far.\n
 *
 */

void lcd_plot_bitmap(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w, uint8_t h) {
	//curent x position in bytes, current y position in bytes, the width of the bitmap in bytes
	uint8_t cx,cy,wb;
	//source and destination position in bytes in the memory
	uint16_t dpos,spos;
	// x offest used if the x coordinate is in the middle of a byte
	uint8_t xoff;
	//  various places to store a byte
	uint8_t fbyte,nbyte,byte;

	// check whether the bitmap fits on the display
	if((x <= LCD_GRAPHIC_WIDTH - 1) && (y <= LCD_GRAPHIC_HEIGHT - 1)
		&& (x + w <= LCD_GRAPHIC_WIDTH - 1) && (y + h <= LCD_GRAPHIC_HEIGHT - 1)) {

		// Calculate the width in byte (1-8 pixel one byte, 9-16 pixel two bytes ..)
		if (w % 8 != 0) {
			wb = w / 8 + 1;
		}
		else {
			wb = w / 8;
		}

		// Loop bytewise through the bitmap
		for(cy = 0; cy < h; cy++) {
			for(cx = 0; cx < wb; cx++) {
				//source and destination positions
				spos = cx + cy * wb;
				dpos = (x  + y * LCD_GRAPHIC_WIDTH) / 8 + (cx  + cy * LCD_GRAPHIC_WIDTH / 8);

				//Crop the Bitmap if it doesn't use all the bits in the last byte
				if((w % 8 != 0) & (cx == wb -1)) {
					byte = pgm_read_byte(bitmap+spos);
					byte = (byte & ~(0xFF << (w % 8)));
				}
				else {
					byte = pgm_read_byte(bitmap+spos);
				}

				//Coordinate is the first bit of a display memory byte
				if(x % 8 == 0) {
					byte = lcd_read_byte(dpos) | byte;
					lcd_write_byte(dpos,byte);
				}
				else { // Coordinate is somewhere in the byte, let's do some shifting
					xoff = x % 8;

					fbyte = byte & ~(0xFF << (8 - xoff));
					nbyte = byte & (0xFF << (8 - xoff));

					byte = lcd_read_byte(dpos) | (fbyte << xoff);
					lcd_write_byte(dpos, byte);

					byte = lcd_read_byte(dpos+1) | (nbyte >> (8 - xoff));
					lcd_write_byte(dpos+1, byte);

				}
			}
		}
	}
};




/**
 * Plots a single character in graphicmode.
 * Each character of the font is stored linewise. \n
 * This means for a 8x8 Pixel font the 8 bytes are needed. \n
 * The first byte is the first line and the last byte is the last line. \n
 * The next 8byte are the next character.\n
 * \n
 * If the position is too close to the display edge so it can't be displayed completely,\n
 * It won't be displayed at all.
 *
 * @param x_off horizontal position of the character
 * @param y_off vertical position of the character
 * @param c the character or it's ascii code
 * @param fw width of the font (we start counting from 0, so a 10px width font will have fw = 9)
 * @param fh height of the font (we start counting from 0, so a 10px height font will have fh = 9)
 * @param font pointer to the flash area where the font is stored
 *
 */
void lcd_plot_char(uint8_t x_off, uint8_t y_off, uint8_t c, uint8_t fw, uint8_t fh, PGM_P font) {
PGM_P letter;
uint8_t fsize,fwb;

	if (fw % 8 != 0) {
		fwb = fw / 8 + 1;
	}
	else {
		fwb = fw / 8;
	}

	fsize = fh * fwb;
	letter = font + c * fsize;

	lcd_plot_bitmap(x_off,y_off,letter,fw,fh);

}

 /**
 * Plots a text using lcd_plot_char.
 * @param x_off horizontal position of the character
 * @param y_off vertical position of the character
 * @param text the text that is plotted
 * @param fw width of the font (we start counting from 0, so a 10px width font will have fw = 9)
 * @param fh height of the font (we start counting from 0, so a 10px height font will have fh = 9)
 * @param font pointer to the flash area where the font is stored
 *
 *
 *
 * @see lcd_plot_char
 */
void lcd_plot_text(uint8_t x_off, uint8_t y_off, const char *text, uint8_t fw, uint8_t fh, PGM_P font) {

while(*text) {
	lcd_plot_char(x_off,y_off,(uint8_t) *text,fw,fh,font);
	x_off += fw;
	text++;
}


}

/**
* Plots a text using lcd_plot_char.
* @param x_off horizontal position of the character
* @param y_off vertical position of the character
* @param text flash area where the text is stored
* @param fw width of the font (we start counting from 0, so a 10px width font will have fw = 9)
* @param fh height of the font (we start counting from 0, so a 10px height font will have fh = 9)
* @param font pointer to the flash area where the font is stored
*
*
*
* @see lcd_plot_char
* @see lcd_plot_text
*/
void lcd_plot_pgmtext(uint8_t x_off, uint8_t y_off, PGM_P text, uint8_t fw, uint8_t fh, PGM_P font) {
    uint8_t c;

	c = pgm_read_byte (text);
	while (c != 0)
    {
		lcd_plot_char(x_off,y_off,c,fw,fh,font);
		x_off += fw;
		text++;
       	c = pgm_read_byte (text);
    }
}



