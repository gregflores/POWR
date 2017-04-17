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
#include "lcd.h"
#include "lcd_layout.h"
#include "HAL_MSP432_320x240_ILI9341.h"

void lcd_pageInit(void);
void lcd_primary(void);
void drawTemp(uint16_t a, uint16_t b);

#endif /* LCD_PAGES_H_ */
