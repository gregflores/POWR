//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************


#include "msp.h"

/* Standard Libraries */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* TI Libraries */
#include "math.h"
#include "driverlib.h"

/* DS18B20 Libraries */
#include "onewire.h"
#include "ds18b20.h"

/* LCD Libraries */
#include "color.h"
#include "graphics.h"
#include "lcd_pages.h"

/* W5500 Libraries */
#include "w5500.h"
#include "msp432server.h"
#include "wizdebug.h"
#include "clientServerMode.h"

/* User Libraries */
#include "mcu_init.h"

/* ADC results buffer */
static uint16_t resultsBuffer[20];
static float normalizedResults[20];
static uint16_t wattsResults[20];

static uint8_t state;
static uint8_t adc_flag;

void main(void)
{
    msp432Init();
    W5500_SpiInit();
	configureW5500(sourceIP, gatewayIP, subnetMask);
	//uint8_t i;
	uint16_t a, b;
    onewire_t ow1, ow2;
    state = 0;
    int8_t string[] = "";


    /* Configuring P1.1 as an input and enabling interrupts */
    keyInputInit();
    adcInit();

    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

    //lcd_pageInit();
    //lcd_primary();
    memset(resultsBuffer, 0x00, 20);

    ow1.port_out = &P6OUT;
    ow1.port_in = &P6IN;
    ow1.port_ren = &P6REN;
    ow1.port_dir = &P6DIR;
    ow1.pin = BIT2;

    ow2.port_out = &P6OUT;
    ow2.port_in = &P6IN;
    ow2.port_ren = &P6REN;
    ow2.port_dir = &P6DIR;
    ow2.pin = BIT3;

	printf("Temp A:");
    a = DS_tempRead(&ow1);

    printf("\nTemp B:");
    b = DS_tempRead(&ow2);

    //drawTemp(a, b);

    MAP_Timer32_setCount(TIMER32_BASE,48000000);
    MAP_Timer32_enableInterrupt(TIMER32_BASE);
    MAP_Timer32_startTimer(TIMER32_BASE, true);
    MAP_Interrupt_enableMaster();
    while(1)
    {
    	switch(state)
    	{
    	case 1:
        	printf("\nTemp A:");
            a = DS_tempRead(&ow1);

            printf("\nTemp B:");
            b = DS_tempRead(&ow2);

            //drawTemp(a, b);
            state = 0;
            MAP_Timer32_setCount(TIMER32_BASE,48000000);
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
			//drawString(68, (34+((adc_flag)*16)), FONT_MD_BKG, string);

			sprintf((char*)string, " %3dW", wattsResults[adc_flag+10]);
			//drawString(148, (34+((adc_flag)*16)), FONT_MD_BKG, string);

			//setColor(COLOR_16_GREEN);
			//fillCircle(210, 39+((adc_flag)*16), 4);
			//setColor(COLOR_16_WHITE);

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
	MAP_Interrupt_disableMaster();
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
    MAP_Interrupt_enableMaster();
}

/* GPIO ISR */
void PORT2_IRQHandler(void)
{
	MAP_Interrupt_disableMaster();
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    if(status & GPIO_PIN6)
    {
        MAP_ADC14_toggleConversionTrigger();
        _delay_cycles(10000);
     	adc_flag = 1;
    }
    else if(status & GPIO_PIN7)
    {
        MAP_ADC14_toggleConversionTrigger();
        _delay_cycles(10000);
     	adc_flag = 2;
    }
    MAP_Interrupt_enableMaster();
}

void PORT3_IRQHandler(void)
{
	MAP_Interrupt_disableMaster();
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    switch(status)
    {
    case GPIO_PIN0:
     	adc_flag = 3;
    	break;
    case GPIO_PIN1:
     	adc_flag = 4;
    	break;
    case GPIO_PIN2:
     	adc_flag = 5;
    	break;
    case GPIO_PIN3:
     	adc_flag = 6;
    	break;
    case GPIO_PIN4:
     	adc_flag = 7;
    	break;
    case GPIO_PIN5:
     	adc_flag = 8;
    	break;
    case GPIO_PIN6:
     	adc_flag = 9;
    	break;
    case GPIO_PIN7:
     	adc_flag = 10;
    	break;
    default:
    	break;
    }
    MAP_ADC14_toggleConversionTrigger();
    _delay_cycles(10000);
    MAP_Interrupt_enableMaster();
}

void T32_INT1_IRQHandler(void)
{
	MAP_Interrupt_disableMaster();
	static uint16_t timer_count = 0;
	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	timer_count++;

	if (timer_count == 60)
	{
		state = 1;
		timer_count = 0;
	}
	else MAP_Timer32_setCount(TIMER32_BASE,48000000);
	MAP_Interrupt_enableMaster();
}

