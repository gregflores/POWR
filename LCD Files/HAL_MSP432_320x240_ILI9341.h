/*
 * mcuLCD.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Gregorio Flores
 */

#ifndef HAL_MSP432_320X240_ILI9341_H_
#define HAL_MSP432_320X240_ILI9341_H_

#include "config.h"
#include <stdint.h>
//
//
void HAL_LCD_SpiInit();
void writeData(uint8_t data);
void writeCommand(uint8_t command);
void delay(uint8_t msec);

#define LCD_EUSCI_MODULE		EUSCI_B0_BASE
#define LCD_EUSCI_REC_INT		EUSCI_B_SPI_RECEIVE_INTERRUPT
#define LCD_EUSCI_TRAN_INT		EUSCI_B_SPI_TRANSMIT_INTERRUPT
#define LCD_INT_ENABLE 			INT_EUSCIB0

// clock P1.5
#ifndef LCD_SCLK_PIN
#define LCD_SCLK_PIN GPIO_PIN5
#define LCD_SCLK_PORT GPIO_PORT_P1
#endif

// data out
#ifndef LCD_MOSI_PIN
#define LCD_MOSI_PIN GPIO_PIN6
#define LCD_MOSI_PORT GPIO_PORT_P1
#endif

// data in
#ifndef LCD_LITE_PIN
#define LCD_LITE_PIN GPIO_PIN7
#define LCD_LITE_PORT GPIO_PORT_P1
#endif

// chip select
#ifndef LCD_CS_PIN
#define LCD_CS_PIN GPIO_PIN5 // P2.5
#define LCD_CS_PORT GPIO_PORT_P2 //
#endif

// data/control
#ifndef LCD_DC_PIN
#define LCD_DC_PIN GPIO_PIN7 // P3.7
#define LCD_DC_PORT GPIO_PORT_P3 //
#endif

#endif /* HAL_MSP432_320X240_ILI9341_H_ */
