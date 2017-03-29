/*
 * config.h
 *
 *  Created on: Jul 12, 2013
 *      Author: RobG
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* supported displays */
#define ILI9341 // 2.2" & 2.4" 320x240

/* comment out to use software SPI */
#define HARDWARE_SPI

/* default orientation */
#define ORIENTATION 1 // 0 1 2 3

/* eUSCI registers */
//#define UCXxTXBUF UCB0TXBUF
//#define UCXxRXBUF UCB0RXBUF
//#define UCXxSTAT UCB0STAT
//#define UCXxBR0 UCB0BR0

#endif /* CONFIG_H_ */
