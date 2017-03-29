/*
 * graphics.h
 *
 *  Created on: Mar 19, 2012
 *      Author: RobG
 */
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
//
// font sizes
#define FONT_SM		0
#define FONT_MD		1
#define FONT_LG		2
#define FONT_SM_BKG	3
#define FONT_MD_BKG	4
#define FONT_LG_BKG	5
//
void setColor(uint16_t color);
void setBackgroundColor(uint16_t color);
//
void clearScreen(uint8_t blackWhite);
void drawPixel(uint16_t x, uint16_t y);
void drawString(uint16_t x, uint16_t y, int8_t type, int8_t *string);
void drawCharLg(uint16_t x, uint16_t y, int8_t c);
void drawCharMd(uint16_t x, uint16_t y, int8_t c);
void drawCharSm(uint16_t x, uint16_t y, int8_t c);
void drawCharLgBkg(uint16_t x, uint16_t y, int8_t c);
void drawCharMdBkg(uint16_t x, uint16_t y, int8_t c);
void drawCharSmBkg(uint16_t x, uint16_t y, int8_t c);
void drawLine(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void drawRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void drawCircle(uint16_t x, uint16_t y, uint8_t radius);
//
void fillRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void fillCircle(uint16_t x, uint16_t y, uint8_t radius);
//
void drawLogicLine(uint16_t x, uint16_t y, uint16_t length, uint8_t height, uint8_t * data);
//
void drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data);
void drawImageLut(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data, uint16_t * lut);
void drawImageMono(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data);
//
uint16_t getPixel(uint16_t x, uint16_t y);
void getPixels(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t * data);
//
//
#endif /* GRAPHICS_H_ */

