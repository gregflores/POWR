/*
 * lcd_pages.c
 *
 *  Created on: Mar 7, 2017
 *      Author: Gregorio Flores
 */

#include "lcd_pages.h"

void lcd_pageInit(void)
{
	HAL_LCD_SpiInit();
	_delay_cycles(160000);
	initLCD();
	clearScreen(1);
}

void lcd_primary()
{
	setColor(COLOR_16_WHITE);
	drawString(COMPANY_NAME_X, COMPANY_NAME_Y, FONT_MD, "MEGAHERTZ");
	drawString(TEAM_NAME_X, TEAM_NAME_Y, FONT_MD, "GIRAF Tech");

	drawString(REPEATER_X, REPEATER_Y_1, FONT_MD, " 1 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_2, FONT_MD, " 2 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_3, FONT_MD, " 3 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_4, FONT_MD, " 4 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_5, FONT_MD, " 5 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_6, FONT_MD, " 6 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_7, FONT_MD, " 7 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_8, FONT_MD, " 8 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_9, FONT_MD, " 9 FWD: 000W  RVS: 000W");
	drawString(REPEATER_X, REPEATER_Y_10, FONT_MD, "10 FWD: 000W  RVS: 000W");

	drawString(TEMP_A_X, TEMP_A_Y, FONT_MD, "TEMP A:");
	drawString(TEMP_A_VAL_X, TEMP_A_VAL_Y , FONT_MD, "000F");

	drawString(TEMP_B_X, TEMP_B_Y, FONT_MD, "TEMP B:");
	drawString(TEMP_B_VAL_X, TEMP_B_VAL_Y, FONT_MD, "000F");

    drawString(IP_X,IP_Y, FONT_MD, " IP:");
	drawString(156, 226, FONT_MD, "Web: GIRAFTECH.XYZ");
}

void drawTemp(uint16_t a, uint16_t b)
{
	int8_t string[] = "";
	sprintf((char*)string, "%3uF", a);

	drawString(TEMP_A_VAL_X, TEMP_A_VAL_Y, FONT_MD_BKG, string);
	sprintf((char*)string, "%3uF", b);

	drawString(TEMP_B_VAL_X, TEMP_B_VAL_Y, FONT_MD_BKG, string);
}
