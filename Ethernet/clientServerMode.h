/*
 * clientServerMode.h
 *
 *  Created on: Apr 18, 2017
 *      Author: flore
 */

#ifndef CLIENTSERVERMODE_H_
#define CLIENTSERVERMODE_H_

#include "msp.h"
#include "driverlib.h"
#include "typedefs.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "w5500.h"
#include "msp432server.h"

void runAsServer();
void runAsClient();
// used for client example
void waitForEvent();


#endif /* CLIENTSERVERMODE_H_ */
