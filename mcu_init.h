/*
 * mcu_init.h
 *
 *  Created on: Mar 7, 2017
 *      Author: Gregorio Flores
 */

#ifndef MCU_INIT_H_
#define MCU_INIT_H_

#define IN_CONTACT_PORT		GPIO_PORT_P7
#define IN_CONTACT_PIN		GPIO_PIN7
#define OUT_CONTACT_PORT	GPIO_PORT_P7
#define OUT_CONTACT_PIN		GPIO_PIN6

void msp432Init(void);
void adcInit(void);
void keyInputInit(void);

#endif /* MCU_INIT_H_ */
