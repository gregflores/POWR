/*
 * mcuLCD.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Gregorio Flores
 */
#include "HAL_MSP432_320x240_ILI9341.h"

#include "msp.h"
#include "lcd.h"
#include "config.h"
#include "driverlib.h"
void HAL_LCD_SpiInit()
{

    MAP_GPIO_setAsOutputPin(LCD_DC_PORT,
                        LCD_DC_PIN);

    MAP_GPIO_setOutputLowOnPin(LCD_DC_PORT,
                           LCD_DC_PIN);

    MAP_GPIO_setAsOutputPin(LCD_CS_PORT,
                        LCD_CS_PIN);

    MAP_GPIO_setOutputLowOnPin(LCD_CS_PORT,
                           LCD_CS_PIN);

    MAP_GPIO_setAsOutputPin(LCD_LITE_PORT,
                        LCD_LITE_PIN);

    MAP_GPIO_setOutputHighOnPin(LCD_LITE_PORT,
                           LCD_LITE_PIN);
    //
    // Configure SPI peripheral.
    //
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_MOSI_PORT,
                                                LCD_MOSI_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);


	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SCLK_PORT,
    											LCD_SCLK_PIN,
												GPIO_PRIMARY_MODULE_FUNCTION);

    eUSCI_SPI_MasterConfig spiMasterConfig =
    {
        EUSCI_SPI_CLOCKSOURCE_SMCLK,                      		// SMCLK Clock Source
        MAP_CS_getSMCLK(),                                  			// Get SMCLK frequency
        8000000,                                                	// SPICLK = 16 MHz
        EUSCI_SPI_MSB_FIRST,                             			// MSB First
		EUSCI_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT, 	// Phase //  EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
        EUSCI_SPI_CLOCKPOLARITY_INACTIVITY_LOW,         			// Low polarity
		EUSCI_SPI_3PIN                                   			// SPI Mode
    };

    SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
    SPI_enableModule(EUSCI_B0_BASE);
    Interrupt_enableInterrupt(LCD_INT_ENABLE);
    SPI_clearInterruptFlag(EUSCI_B0_BASE,  EUSCI_B_SPI_RECEIVE_INTERRUPT);
}


void writeData(uint8_t data)
{
    // Wait for the transmit buffer to become empty.
	while (!(SPI_getInterruptStatus(LCD_EUSCI_MODULE,LCD_EUSCI_TRAN_INT)));
	SPI_transmitData(LCD_EUSCI_MODULE, data);
	_delay_cycles(39);
}


void writeCommand(uint8_t command)
{
    // Wait for any SPI transmission to complete before setting the LCD_DC signal.
	while (!(SPI_getInterruptStatus(LCD_EUSCI_MODULE,LCD_EUSCI_TRAN_INT)));

	// Set the LCD_DC signal low, indicating that following writes are commands.
    GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN);

    // Transmit the command.
    SPI_transmitData(LCD_EUSCI_MODULE, command);

    // Wait for the SPI transmission to complete before setting the LCD_SDC signal.
	while (!(SPI_getInterruptStatus(LCD_EUSCI_MODULE,LCD_EUSCI_TRAN_INT)));
	_delay_cycles(39);
    // Set the LCD_SDC signal high, indicating that following writes are data.
    GPIO_setOutputHighOnPin(LCD_DC_PORT,LCD_DC_PIN);
}



void delay(uint8_t x10ms)
{
	while (x10ms > 0)
	{
		_delay_cycles(48000);
		x10ms--;
	}
}
