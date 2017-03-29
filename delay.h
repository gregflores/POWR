/*
 * delay.h
 *
 *  Created on: Mar 28, 2017
 *      Author: flore
 */

#ifndef DELAY_H_
#define DELAY_H_

#define CYCLES_PER_US 48L  //48MHz
#define CYCLES_PER_MS (CYCLES_PER_US * 1000L)

#define DELAY_US(x) _delay_cycles((x * CYCLES_PER_US))
#define DELAY_MS(x) _delay_cycles((x * CYCLES_PER_MS))

#endif /* DELAY_H_ */
