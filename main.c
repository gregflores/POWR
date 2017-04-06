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
#include "mcu_init.h"
#include "color.h"
#include "graphics.h"
#include <string.h>
#include "lcd_pages.h"
#include "math.h"

/* ADC results buffer */
static uint16_t resultsBuffer[20];

static float normalizedResults[20];
static uint16_t wattsResults[20];
//static bool lcd_flag = false;

uint8_t state;
uint8_t adc_flag;
uint8_t timer_count = 0;
void main(void)
{
    msp432Init();
	//uint8_t i;
	uint16_t a, b;
    onewire_t ow1, ow2;
    state = 0;
    int8_t string[] = "";

    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

    lcd_pageInit();
    lcd_primary();
    memset(resultsBuffer, 0x00, 20);
    adcInit();
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

	printf("On Board:");
    a = DS_tempRead(&ow1);

    printf("\nWire:");
    b = DS_tempRead(&ow2);

    drawTemp(a, b);

    MAP_Timer32_setCount(TIMER32_BASE,48000000*60);
    MAP_Timer32_enableInterrupt(TIMER32_BASE);
    MAP_Timer32_startTimer(TIMER32_BASE, true);

    while(1)
    {
    	switch(state)
    	{
    	case 1:
        	printf("\nOn Board:");
            a = DS_tempRead(&ow1);

            printf("\nWire:");
            b = DS_tempRead(&ow2);

            drawTemp(a, b);
            state = 0;
            MAP_Timer32_setCount(TIMER32_BASE,48000000*60);
            MAP_Timer32_enableInterrupt(TIMER32_BASE);
            MAP_Timer32_startTimer(TIMER32_BASE, true);

    		break;
    	case 2:

    		//y=0.5657x-0.1766
    		//wattsResults = ((normalizedResults[n]*4)+0.1766)/0.5657
			normalizedResults[adc_flag] = (resultsBuffer[adc_flag] * 3.3) / 16384;
			normalizedResults[adc_flag+10] = (resultsBuffer[adc_flag+10] * 3.3) / 16384;
			wattsResults[adc_flag] = pow(10,(log10((normalizedResults[adc_flag]*4))+0.1766)/0.5657);
			wattsResults[adc_flag + 10] = pow(10, (log10((normalizedResults[adc_flag + 10]*4))+0.1766)/0.5657);

			sprintf((char*)string, "%3dW", wattsResults[adc_flag]); //%.2f normalizedResults
			drawString(68, (34+((adc_flag)*16)), FONT_MD_BKG, string);

			sprintf((char*)string, " %3dW", wattsResults[adc_flag+10]);
			drawString(148, (34+((adc_flag)*16)), FONT_MD_BKG, string);

			setColor(COLOR_16_GREEN);
			fillCircle(210, 39+((adc_flag)*16), 4);
			setColor(COLOR_16_WHITE);

			state = 0;
    		break;
    	default:
    		break;
    	}
    }
}

/***************************************************************/

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEMx. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT19)
    {
        /* Store ADC14 conversion results */
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
        state = 2;
    }
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
     	_delay_cycles(10000);
        MAP_ADC14_toggleConversionTrigger();
        _delay_cycles(10000);
     	adc_flag = 8;

    }
    if(status & GPIO_PIN4)
    {
        state = 1;
    }

}


void T32_INT1_IRQHandler(void)
{
	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	timer_count++;
	if (timer_count == 1)
	{
		state = 1;
		timer_count = 0;
	}
}

