/*
 * lcd_pages.h
 *
 *  Created on: Mar 7, 2017
 *      Author: Gregorio Flores
 */

#ifndef LCD_PAGES_H_
#define LCD_PAGES_H_


#include "color.h"
#include "graphics.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcd_pages.h"

void lcd_pageInit(void);
void lcd_primary(void);
void drawTemp(uint16_t a, uint16_t b);

#endif /* LCD_PAGES_H_ */
