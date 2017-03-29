/*
 * lcd.c
 *
 *  Created on: Jul 12, 2013
 *      Author: RobG
 */
#include "lcd.h"
#include "config.h"
#include "HAL_MSP432_320x240_ILI9341.h"

uint8_t _orientation = 2;

uint16_t getScreenWidth() {
	if (_orientation == 0 || _orientation == 2)
		return SHORT_EDGE_PIXELS;
	else
		return LONG_EDGE_PIXELS;
}

uint16_t getScreenHeight() {
	if (_orientation == 0 || _orientation == 2)
		return LONG_EDGE_PIXELS;
	else
		return SHORT_EDGE_PIXELS;
}

void setArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {

	uint8_t yStartMSB = yStart >> 8;
	uint8_t yEndMSB = yEnd >> 8;
	uint8_t xStartMSB = xStart >> 8;
	uint8_t xEndMSB = xEnd >> 8;

	writeCommand(CASETP);

	writeData(xStartMSB);
	writeData(xStart);

	writeData(xEndMSB);
	writeData(xEnd);

	writeCommand(PASETP);

	writeData(yStartMSB);
	writeData(yStart);

	writeData(yEndMSB);
	writeData(yEnd);

	writeCommand(RAMWRP);
	// data to follow
}

////////////////////////////////////
// gamma, lut, and other inits
////////////////////////////////////

/////////////////////////////////////////////////
// ILI9340 based display
/////////////////////////////////////////////////
#ifdef ILI9341

void initLCD() {
	writeCommand(0xEF);
	writeData(0x03);
	writeData(0x80);
	writeData(0x02);

	writeCommand(PWRCTRLB);
	writeData(0x00);
	writeData(0XC1);
	writeData(0X30);

	writeCommand(POSC);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);

	writeCommand(DTCTRLA1);
	writeData(0x85);
	writeData(0x00);
	writeData(0x78);

	writeCommand(PWRCTRLA);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);

	writeCommand(PRC);
	writeData(0x20);

	writeCommand(DTCTRLB);
	writeData(0x00);
	writeData(0x00);

	writeCommand(ILIPC1);
	writeData(0x23);
	writeCommand(ILIPC2);
	writeData(0x10);
	writeCommand(ILIVC1);
	writeData(0x3E);
	writeData(0x28);
	writeCommand(ILIVC2);
	writeData(0x86);

	setOrientation(ORIENTATION);

	writeCommand(COLMOD);
	writeData(0x55);

	writeCommand(ILIFCNM);
	writeData(0x00);
	writeData(0x18);

	writeCommand(ILIDFC);
	writeData(0x08);
	writeData(0x82);
	writeData(0x27);

	writeCommand(ILIGFD);
	writeData(0x00);

	writeCommand(ILIGS);
	writeData(0x01);

	writeCommand(ILIPGC);
	const uint8_t gamma1[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
			0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
	uint8_t c = 0;
	while (c < 15) {
		writeData(gamma1[c]);
		c++;
	}

	writeCommand(ILINGC);
	const uint8_t gamma2[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
			0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
	c = 0;
	while (c < 15) {
		writeData(gamma2[c]);
		c++;
	}

	writeCommand(SLEEPOUT);
	delay(120);
	writeCommand(DISPON);
	writeCommand(RAMWRP);

}

void setOrientation(uint8_t orientation) {

	writeCommand(ILIMAC);

	switch (orientation) {
	case 1:
		writeData(0xE8);
		_orientation = 1;
		break;
	case 2:
		writeData(0x88);
		_orientation = 2;
		break;
	case 3:
		writeData(0x28);
		_orientation = 3;
		break;
	default:
		writeData(0x48);
		_orientation = 0;
	}
}

#endif


