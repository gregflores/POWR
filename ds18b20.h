/*
 * ds18b20.h
 *
 *  Created on: Mar 29, 2017
 *      Author: flore
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "msp.h"

#include <stdio.h>
#include <stdint.h>
#include "driverlib.h"
#include "onewire.h"
#include "delay.h"


#define TEMP_DEBUG 1

uint16_t DS_tempRead(onewire_t *ow);
void search(onewire_t *ow, uint8_t *id, int depth, int reset);

#endif /* DS18B20_H_ */
