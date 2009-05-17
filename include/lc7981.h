/*
 * lc7981.h
 *
 *  Created on: 01.05.2009
 *      Author: sebastian
 *
 *    This file is part of Sebastians AVR Library for lc7981.
 *
 *   Sebastians AVR Library is free software: you can redistribute it and/or modify
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
 */

#ifndef LC7981_H_
#define LC7981_H_ LC7981_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define LCD_CTRL 	PORTC
#define LCD_CRTL_DDR	DDRC
#define LCD_RS			PC4
#define LCD_RW			PC2
#define LCD_EN			PC0



#define LCD_DATA		PORTD
#define LCD_DATA_DDR	DDRD

// Macros for (un)setting the control pins
#define lcd_rs_high() (LCD_CTRL |= (1 << LCD_RS))
#define lcd_rs_low() (LCD_CTRL &= ~(1 << LCD_RS))

#define lcd_rw_high() (LCD_CTRL |= (1 << LCD_RW))
#define lcd_rw_low() (LCD_CTRL &= ~(1 << LCD_RW))

#define lcd_en_high() (LCD_CTRL |= (1 << LCD_EN))
#define lcd_en_low() (LCD_CTRL &= ~(1 << LCD_EN))


#define LCD_TEXT 0
#define LCD_TEXT_LINES 10
#define LCD_TEXT_COLUMNS 26

#define LCD_GRAPHIC 1
#define LCD_GRAPHIC_WIDTH 160
#define LCD_GRAPHIC_HEIGHT 80
#define PIXEL_ON 1
#define PIXEL_OFF 0

void lcd_init(uint8_t mode);
inline void lcd_clear();

void lcd_write_text(char *txt);
inline void lcd_gotoxy(uint8_t x, uint8_t y);

void lcd_plot_pixel(uint8_t x, uint8_t y, uint8_t set);
void lcd_plot_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h);


inline void lcd_strobe();
inline void lcd_write_command(uint8_t cmd, uint8_t data);
inline uint8_t lcd_read_byte();

#endif /* LC7981_H_ */
