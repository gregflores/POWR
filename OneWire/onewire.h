/*
 * onewire.h
 *
 *  Created on: Mar 28, 2017
 *      Author: flore
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdint.h>

typedef struct {
    volatile uint8_t *port_out;
    const volatile uint8_t *port_in;
    volatile uint8_t *port_ren;
    volatile uint8_t *port_dir;
    int pin;
  } onewire_t;

//########################################################################

int onewire_reset(onewire_t *ow);
void onewire_write_bit(onewire_t *ow, int bit);
int onewire_read_bit(onewire_t *ow);
void onewire_write_byte(onewire_t *ow, uint8_t byte);
uint8_t onewire_read_byte(onewire_t *ow);
void onewire_line_low(onewire_t *ow);
void onewire_line_high(onewire_t *ow);
void onewire_line_release(onewire_t *ow);

#endif /* ONEWIRE_H_ */
