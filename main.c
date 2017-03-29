//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include <stdio.h>
#include <stdint.h>
#include "driverlib.h"
#include "onewire.h"
#include "ds18b20.h"

void search(onewire_t *ow, uint8_t *id, int depth, int reset);

void main(void)
{
    onewire_t ow1, ow2;

    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();
    FPU_enableModule();
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setPowerState(PCM_AM_DCDC_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_setDCOFrequency(48000000);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);


    ow1.port_out = &P6OUT;
    ow1.port_in = &P6IN;
    ow1.port_ren = &P6REN;
    ow1.port_dir = &P6DIR;
    ow1.pin = BIT4;

    ow2.port_out = &P6OUT;
    ow2.port_in = &P6IN;
    ow2.port_ren = &P6REN;
    ow2.port_dir = &P6DIR;
    ow2.pin = BIT5;

    while(1)
    {
        DS_tempRead(&ow2);
        _delay_cycles(48000000);
    }
}

/***************************************************************/







