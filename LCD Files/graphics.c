/*
 * graphics.c
 *
 *  Created on: July 20, 2014
 *      Author: RobG
 */

#include "graphics.h"
#include "fonts.h"

extern void setArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
extern void writeData(uint8_t data);
extern void readDataBegin(uint8_t command);
extern uint8_t readData();
extern void readDataEnd();
extern uint16_t getScreenWidth();
extern uint16_t getScreenHeight();

//
uint8_t colorLowByte = 0;
uint8_t colorHighByte = 0;
uint8_t bgColorLowByte = 0;
uint8_t bgColorHighByte = 0;

//////////////////////
// color
//////////////////////

void setColor(uint16_t color) {
	colorLowByte = color;
	colorHighByte = color >> 8;
}

void setBackgroundColor(uint16_t color) {
	bgColorLowByte = color;
	bgColorHighByte = color >> 8;
}

/////////////////
// drawing
/////////////////

void clearScreen(uint8_t blackWhite) {
	setArea(0, 0, getScreenWidth() - 1, getScreenHeight() - 1);
	setBackgroundColor(blackWhite ? 0x0000 : 0xFFFF);
	uint16_t w = getScreenWidth();
	uint16_t h = getScreenHeight();
	while (h != 0) {
		while (w != 0) {
			writeData(bgColorHighByte);
			writeData(bgColorLowByte);
			w--;
		}
		w = getScreenWidth();
		h--;
	}
}

void drawPixel(uint16_t x, uint16_t y) {
	setArea(x, y, x, y);
	writeData(colorHighByte);
	writeData(colorLowByte);
}

/////////////////////////////
// Draw String - type: 0=Sm, 1=Md, 2=Lg, 3=Sm/Bkg, 4=Md/Bkg, 5=Lg/Bkg
/////////////////////////////
void drawString(uint16_t x, uint16_t y, int8_t type, int8_t *string) {
	uint16_t xs = x;
	switch (type) {
	case FONT_SM:
		while (*string) {
			drawCharSm(xs, y, *string++);
			xs += 6;
		}
		break;
	case FONT_MD:
		while (*string) {
			drawCharMd(xs, y, *string++);
			xs += 8;
		}
		break;
	case FONT_LG:
		while (*string) {
			drawCharLg(xs, y, *string++);
			xs += 12;
		}
		break;
	case FONT_SM_BKG:
		while (*string) {
			drawCharSmBkg(xs, y, *string++);
			xs += 6;
		}
		break;
	case FONT_MD_BKG:
		while (*string) {
			drawCharMdBkg(xs, y, *string++);
			xs += 8;
		}
		break;
	case FONT_LG_BKG:
		while (*string) {
			drawCharLgBkg(xs, y, *string++);
			xs += 12;
		}
		break;
	}
}

//////////////////////////
// 5x7 font - this function does not draw background pixels
//////////////////////////
void drawCharSm(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint8_t bit = 0x01;
	uint8_t oc = c - 0x20;
	while (row < 8) {
		while (col < 5) {
			if (font_5x7[oc][col] & bit)
				drawPixel(x + col, y + row);
			col++;
		}
		col = 0;
		bit <<= 1;
		row++;
	}
}

////////////////
// 5x7 font - this function draws background pixels
////////////////
void drawCharSmBkg(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint8_t bit = 0x01;
	uint8_t oc = c - 0x20;
	setArea(x, y, x + 4, y + 7); // if you want to fill column between chars, change x + 4 to x + 5

	while (row < 8) {
		while (col < 5) {
			if (font_5x7[oc][col] & bit) {
				//foreground
				writeData(colorHighByte);
				writeData(colorLowByte);
			} else {
				//background
				writeData(bgColorHighByte);
				writeData(bgColorLowByte);
			}
			col++;
		}
		// if you want to fill column between chars, writeData(bgColor) here
		col = 0;
		bit <<= 1;
		row++;
	}
}

////////////////
// 11x16 font - this function does not draw background pixels
////////////////
void drawCharLg(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint16_t bit = 0x0001;
	uint8_t oc = c - 0x20;
	while (row < 16) {
		while (col < 11) {
			if (font_11x16[oc][col] & bit)
				drawPixel(x + col, y + row);
			col++;
		}
		col = 0;
		bit <<= 1;
		row++;
	}
}

////////////////
// 11x16 font - this function draws background pixels
////////////////
void drawCharLgBkg(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint16_t bit = 0x0001;
	uint8_t oc = c - 0x20;
	setArea(x, y, x + 10, y + 15);
	while (row < 16) {
		while (col < 11) {
			if (font_11x16[oc][col] & bit) {
				//foreground
				writeData(colorHighByte);
				writeData(colorLowByte);
			} else {
				//background
				writeData(bgColorHighByte);
				writeData(bgColorLowByte);
			}
			col++;
		}
		col = 0;
		bit <<= 1;
		row++;
	}
}

////////////////
// 8x12 font - this function does not draw background pixels
////////////////
void drawCharMd(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint8_t bit = 0x80;
	uint8_t oc = c - 0x20;
	while (row < 12) {
		while (col < 8) {
			if (font_8x12[oc][row] & bit)
				drawPixel(x + col, y + row);
			bit >>= 1;
			col++;
		}
		bit = 0x80;
		col = 0;
		row++;
	}
}

////////////////
// 8x12 font - this function draws background pixels
////////////////
void drawCharMdBkg(uint16_t x, uint16_t y, int8_t c) {
	uint8_t col = 0;
	uint8_t row = 0;
	uint8_t bit = 0x80;
	uint8_t oc = c - 0x20;
	setArea(x, y, x + 7, y + 11);
	while (row < 12) {
		while (col < 8) {
			if (font_8x12[oc][row] & bit) {
				//foreground
				writeData(colorHighByte);
				writeData(colorLowByte);
			} else {
				//background
				writeData(bgColorHighByte);
				writeData(bgColorLowByte);
			}
			bit >>= 1;
			col++;
		}
		bit = 0x80;
		col = 0;
		row++;
	}

}

////////////////////////
// images
////////////////////////
//data is 16 bit color
void drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t * data) {

}

// lut is used, ?0 means skip, sort of a mask?
void drawImageLut(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data,
		uint16_t * lut) {

}

// each bit represents color, fg and bg colors are used, ?how about 0 as a mask?
void drawImageMono(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data) {

}

////////////////////////
// shapes
////////////////////////

void drawLogicLine(uint16_t x, uint16_t y, uint16_t length, uint8_t height, uint8_t * data) {
	uint8_t last = (*data & 0x80);
	uint16_t counter = 0;
	while (counter < length) {
		uint8_t bitCounter = 0;
		uint8_t byte = *data;
		while (bitCounter < 8 && counter < length) {
			if (last == (byte & 0x80)) {
				//draw pixel
				uint16_t h = (byte & 0x80) ? (height + y) : y;
				drawPixel(x + counter, h);
			} else {
				// draw line
				if (byte & 0x80) {
					drawLine(x + counter - 1, y, x + counter, y + height);
				} else {
					drawLine(x + counter - 1, y + height, x + counter, y);
				}
			}
			last = byte & 0x80;
			byte <<= 1;
			bitCounter++;
			counter++;
		}
		*data++;
	}
}

void drawLine(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {

	uint16_t x0, x1, y0, y1;
	uint16_t d = 0;

// handle direction
	if (yStart > yEnd) {
		y0 = yEnd;
		y1 = yStart;
	} else {
		y1 = yEnd;
		y0 = yStart;
	}

	if (xStart > xEnd) {
		x0 = xEnd;
		x1 = xStart;
	} else {
		x1 = xEnd;
		x0 = xStart;
	}

// check if horizontal
	if (y0 == y1) {
		d = x1 - x0 + 1;
		setArea(x0, y0, x1, y1);
		while (d-- > 0) {
			writeData(colorHighByte);
			writeData(colorLowByte);
		}

	} else if (x0 == x1) { // check if vertical
		d = y1 - y0 + 1;
		setArea(x0, y0, x1, y1);
		while (d-- > 0) {
			writeData(colorHighByte);
			writeData(colorLowByte);
		}

	} else { // angled
		int dx, dy;
		int sx, sy;

		if (xStart < xEnd) {
			sx = 1;
			dx = xEnd - xStart;
		} else {
			sx = -1;
			dx = xStart - xEnd;
		}

		if (yStart < yEnd) {
			sy = 1;
			dy = yEnd - yStart;
		} else {
			sy = -1;
			dy = yStart - yEnd;
		}

		int e1 = dx - dy;
		int e2;

		while (1) {
			drawPixel(xStart, yStart);
			if (xStart == xEnd && yStart == yEnd)
				break;
			e2 = 2 * e1;
			if (e2 > -dy) {
				e1 = e1 - dy;
				xStart = xStart + sx;
			}
			if (e2 < dx) {
				e1 = e1 + dx;
				yStart = yStart + sy;
			}
		}
	}
}

void drawRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {

	drawLine(xStart, yStart, xEnd, yStart);
	drawLine(xStart, yEnd, xEnd, yEnd);
	drawLine(xStart, yStart, xStart, yEnd);
	drawLine(xEnd, yStart, xEnd, yEnd);
}

void drawCircle(uint16_t x, uint16_t y, uint8_t radius) {
	int dx = radius;
	int dy = 0;
	int xChange = 1 - 2 * radius;
	int yChange = 1;
	int radiusError = 0;
	while (dx >= dy) {
		drawPixel(x + dx, y + dy);
		drawPixel(x - dx, y + dy);
		drawPixel(x - dx, y - dy);
		drawPixel(x + dx, y - dy);
		drawPixel(x + dy, y + dx);
		drawPixel(x - dy, y + dx);
		drawPixel(x - dy, y - dx);
		drawPixel(x + dy, y - dx);
		dy++;
		radiusError += yChange;
		yChange += 2;
		if (2 * radiusError + xChange > 0) {
			dx--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

/////////////////////////
// fill
/////////////////////////

void fillRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
	setArea(xStart, yStart, xEnd, yEnd);
	uint16_t x = xEnd - xStart + 1;
	uint16_t y = yEnd - yStart + 1;
	uint16_t z = y;
	while (x != 0) {
		while (z != 0) {
			writeData(colorHighByte);
			writeData(colorLowByte);
			z--;
		}
		z = y;
		x--;
	}
}

void fillCircle(uint16_t x, uint16_t y, uint8_t radius) {
	int dx = radius;
	int dy = 0;
	int xChange = 1 - 2 * radius;
	int yChange = 1;
	int radiusError = 0;
	while (dx >= dy) {
		drawLine(x + dy, y + dx, x - dy, y + dx);
		drawLine(x - dy, y - dx, x + dy, y - dx);
		drawLine(x - dx, y + dy, x + dx, y + dy);
		drawLine(x - dx, y - dy, x + dx, y - dy);
		dy++;
		radiusError += yChange;
		yChange += 2;
		if (2 * radiusError + xChange > 0) {
			dx--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

//////////////////////////////
// get pixels
//////////////////////////////
/*
uint16_t getPixel(uint16_t x, uint16_t y) {
	setArea(x, y, x, y);
	readDataBegin(0x2E);
	readData();
	uint16_t color = 0;
	uint8_t temp = 0;
	color = readData();
	color &= 0x00F8;
	color <<= 5;
	color |= readData();
	color &= 0xFFFC;
	color <<= 3;
	temp = readData();
	temp >>= 3;
	color |= temp;
	readDataEnd();
	return color;
}

void getPixels(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd,
		uint16_t * data) {
	setArea(xStart, yStart, xEnd, yEnd);
	uint16_t x = xEnd - xStart + 1;
	uint16_t y = yEnd - yStart + 1;
	uint16_t z = y;
	readDataBegin(0x2E);
	readData();
	while (x != 0) {
		while (z != 0) {
			uint16_t temp1 = 0;
			uint8_t temp2 = 0;
			temp1 = readData();
			temp1 &= 0x00F8;
			temp1 <<= 5;
			temp1 |= readData();
			temp1 &= 0xFFFC;
			temp1 <<= 3;
			temp2 = readData();
			temp2 >>= 3;
			temp1 |= temp2;
			*data++ = temp1;
			z--;
		}
		z = y;
		x--;
	}
	readDataEnd();
}
*/
///////////////////////////////////////
// Misc
///////////////////////////////////////



