/*
 * lc7981.h
 *
 *  Created on: 01.05.2009
 *      Author: sebastian
 *
 *    This file is part of Sebastians AVR Library for lc7981.
 *
 *   Sebastians AVR Library for lc789 is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Sebastians AVR Library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Sebastians AVR Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *			'Do the thing you want to do and let your spirits guide you through.
 *			Live a life, worth being remembered.'
 *			-- Remembered - Firewind --
 */




#ifndef LC7981_H_
#define LC7981_H_ LC7981_H_ 	//!< protects this file from beeing double included

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define LCD_CTRL 	PORTA		//!< Port used for control signals
#define LCD_CRTL_DDR	DDRA 	//!< Data-Direction-Register for the control signals
#define LCD_RS			PA4		//!< Register-Select Pin of the Display
#define LCD_RW			PA2		//!< Read-Write-select Pin
#define LCD_EN			PA0		//!< Strobe Pin



#define LCD_DATA		PORTD	//!< Port used for data
#define LCD_DATA_DDR	DDRD	//!< Data-Direction-Register for Data

// Macros for (un)setting the control pins
#define lcd_rs_high() (LCD_CTRL |= (1 << LCD_RS))	//!< Set the Register-Select pin high
#define lcd_rs_low() (LCD_CTRL &= ~(1 << LCD_RS))	//!< Set the Register-Select pin low

#define lcd_rw_high() (LCD_CTRL |= (1 << LCD_RW))	//!< Set the Read-Write-Select pin high
#define lcd_rw_low() (LCD_CTRL &= ~(1 << LCD_RW))	//!< Set the Register-Select pin low

#define lcd_en_high() (LCD_CTRL |= (1 << LCD_EN))	//!< Set the strobe pin high
#define lcd_en_low() (LCD_CTRL &= ~(1 << LCD_EN))	//!< Set the strobe pin low


#define LCD_TEXT 0				//!< Constant for text mode
#define LCD_TEXT_LINES 10 		//!< Number of lines in text mode
#define LCD_TEXT_COLUMNS 26		//!< Number of columns in text mode

#define LCD_GRAPHIC 1				//!< Constant for graphic mode
#define LCD_GRAPHIC_WIDTH 160		//!< Horizontal display size in pixels
#define LCD_GRAPHIC_HEIGHT 80		//!< Vertical display size in pixels
#define PIXEL_ON 1					//!< see lcd_plot_pixel
#define PIXEL_OFF 0					//!< see lcd_plot_pixel

void lcd_init(uint8_t mode);
inline void lcd_clear();

void lcd_write_text(char *txt);
inline void lcd_gotoxy(uint8_t x, uint8_t y);

void lcd_plot_pixel(uint8_t x, uint8_t y, uint8_t set);
void lcd_plot_bitmap(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w, uint8_t h);

inline void lcd_plot_char(uint8_t x_off, uint8_t y_off, uint8_t c, uint8_t fw, uint8_t fh, PGM_P font);
void lcd_plot_text(uint8_t x_off, uint8_t y_off, const char *text, uint8_t fw, uint8_t fh, PGM_P font);
void lcd_plot_pgmtext(uint8_t x_off, uint8_t y_off, PGM_P text, uint8_t fw, uint8_t fh, PGM_P font);


inline void lcd_strobe();
inline void lcd_write_command(uint8_t cmd, uint8_t data);
inline uint8_t lcd_read_byte();

#endif /* LC7981_H_ */
