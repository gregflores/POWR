/*
 * lcd_pages.c
 *
 *  Created on: Mar 7, 2017
 *      Author: Gregorio Flores
 */

#include "lcd_pages.h"
#include "HAL_MSP432_320x240_ILI9341.h"
#include "color.h"
#include "graphics.h"
#include "lcd.h"

void lcd_pageInit(void)
{
	HAL_LCD_SpiInit();
	_delay_cycles(160000);
	initLCD();
	clearScreen(1);
}
void lcd_primary(void)
{
	setColor(COLOR_16_WHITE);
	drawString(4, 2, FONT_MD, "MEGAHERTZ");
	drawString(236, 2, FONT_MD, "GIRAF Tech");

	drawString(4, 34, FONT_MD, " 1");
	drawString(4, 50, FONT_MD, " 2");
	drawString(4, 66, FONT_MD, " 3");
	drawString(4, 82, FONT_MD, " 4");
	drawString(4, 98, FONT_MD, " 5");
	drawString(4, 114, FONT_MD, " 6");
	drawString(4, 130, FONT_MD, " 7");
	drawString(4, 146, FONT_MD, " 8");
	drawString(4, 162, FONT_MD, " 9");
	drawString(4, 178, FONT_MD, "10");

	drawString(4, 210, FONT_MD, "TEMP  Inside:");
	drawString(116, 210, FONT_MD, "000F");
	drawString(4, 226, FONT_MD, "TEMP Outside:");
	drawString(116, 226, FONT_MD, "000F");

	drawString(156, 210, FONT_MD, " IP: 255.255.255.255");
	drawString(156, 226, FONT_MD, "Web: GIRAFTECH.XYZ");

	drawString(28, 34, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 50, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 66, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 82, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 98, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 114, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 130, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 146, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 162, FONT_MD, "FWD: 000V  RVS: 000V");
	drawString(28, 178, FONT_MD, "FWD: 000V  RVS: 000V");
}


