/*
 * ds18b20.c
 *
 *  Created on: Mar 29, 2017
 *      Author: flore
 */


#include "ds18b20.h"

uint16_t DS_tempRead(onewire_t *ow)
{
    uint8_t scratchpad[9];
    uint16_t sign, temp, temp_c, temp_f, wholeTemp;
    uint8_t temp_lsb,temp_msb, i;

    onewire_reset(ow);
    onewire_write_byte(ow, 0xcc); // skip ROM command
    onewire_write_byte(ow, 0x44); // convert T command
    onewire_line_high(ow);
    DELAY_MS(800); // at least 750 ms for the default 12-bit resolution
    onewire_reset(ow);
    onewire_write_byte(ow, 0xcc); // skip ROM command
    onewire_write_byte(ow, 0xbe); // read scratchpad command

    for (i = 0; i < 9; i++)
    {
    	scratchpad[i] = onewire_read_byte(ow); //Read the scratchpad bytes
    }
#if TEMP_DEBUG
    printf("\nTemp: 0x%x%x\n",scratchpad[1], scratchpad[0]);
#endif
    temp_msb = scratchpad[1];
    temp_lsb = scratchpad[0];
    temp = (temp_msb << 8) + temp_lsb; //Create one 16bit number to modify
    sign = temp & 0xF800; //sign is 0b11111000,00000000
    if (sign == 0xF800)
    {
    	temp = (~temp)+1; //2s complement !!UNTESTED
    }
    wholeTemp = (6 * temp) + (temp / 4); //Multiply by 6.25
    temp_c = wholeTemp/100; //Take only the whole number
#if TEMP_DEBUG
    printf( "\nTempC= %d degrees C", temp_c); // print temp. C
#endif
    temp_f = ((temp_c)* 9)/5 + 32;
#if TEMP_DEBUG
    printf( "\nTempF= %d degrees F\n", temp_f ); // print temp. F
#endif

    return temp_f;
}

/***************************************************************/

void search(onewire_t *ow, uint8_t *id, int depth, int reset)
{
  int i, b1, b2;

  if (depth == 64)
  {
    // we have all 64 bit in this search branch
    printf("found: ");
    for (i = 0; i < 8; i++) printf("%02x", id[i]);
    printf("\n");
    return;
  }

  if (reset)
  {
    if (onewire_reset(ow) != 0) { printf("reset failed\n"); return; }
    onewire_write_byte(ow, 0xF0); // search ROM command

    // send currently recognized bits
    for (i = 0; i < depth; i++)
    {
      b1 = onewire_read_bit(ow);
      b2 = onewire_read_bit(ow);
      onewire_write_bit(ow, id[i / 8] & (1 << (i % 8)));
    }
  }

  // check another bit
  b1 = onewire_read_bit(ow);
  b2 = onewire_read_bit(ow);
  if (b1 && b2) return; // no response to search
  if (!b1 && !b2) // two devices with different bits on this position
  {
    // check devices with this bit = 0
    onewire_write_bit(ow, 0);
    id[depth / 8] &= ~(1 << (depth % 8));
    search(ow, id, depth + 1, 0);
    // check devices with this bit = 1
    id[depth / 8] |= 1 << (depth % 8);
    search(ow, id, depth + 1, 1); // different branch, reset must be issued
  } else if (b1) {
    // devices have 1 on this position
    onewire_write_bit(ow, 1);
    id[depth / 8] |= 1 << (depth % 8);
    search(ow, id, depth + 1, 0);
  } else if (b2) {
    // devices have 0 on this position
    onewire_write_bit(ow, 0);
    id[depth / 8] &= ~(1 << (depth % 8));
    search(ow, id, depth + 1, 0);
  }
}


