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

/* defines */
#define TEMP_READ   1
#define ADC_READ    2
#define SEND_DATA   3
#define DEFAULT     0

#define INPUT_ON    0
#define INPUT_OFF   1

/* ADC results buffer */
static uint16_t resultsBuffer[20];
static float normalizedResults[20];
uint16_t wattsResults[20];

/* Status flag for the input contact */
uint8_t inputStatus = 1;

/* state flag  */
static uint8_t state = DEFAULT;

/* which adc to check */
static uint8_t adc_flag;

/* temp variables for the temperature sensors */
uint16_t a, b;

/* network configuration */
const u_char sourceIP[4] = { 192, 168, 1, 10 }; // local IP
const u_char gatewayIP[4] = { 192, 168, 1, 1 }; // gateway IP
const u_char subnetMask[4] = { 255, 255, 255, 0 }; // subnet mask

/* network configuration for client mode */
const u_char destinationIP[4] = { 192, 168, 1, 2 }; // destination IP
const u_int destinationPort = 23; // destination port

void main(void)
{
    msp432Init();
    W5500_SpiInit();
    keyInputInit();
    adcInit();
    lcd_pageInit();
    lcd_primary();

    onewire_t ow1, ow2;
    //state = 0;
    int8_t string[] = "";

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
    
    configureW5500(sourceIP, gatewayIP, subnetMask);

    MAP_Interrupt_enableMaster();

    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableInterrupt(INT_PORT3);
    MAP_Interrupt_enableInterrupt(INT_PORT2);

	drawString(IP_VAL_X, IP_VAL_Y, FONT_MD, "192.168.1.10");

    while(1)
    {
    	switch(state)
    	{
    	case TEMP_READ:
    		MAP_Interrupt_disableMaster();
        	printf("\nTemp A:");
            a = DS_tempRead(&ow1);

            printf("\nTemp B:");
            b = DS_tempRead(&ow2);

            drawTemp(a, b);

            state = DEFAULT;
            MAP_Timer32_setCount(TIMER32_BASE,48000000);
            MAP_Timer32_enableInterrupt(TIMER32_BASE);
            MAP_Timer32_startTimer(TIMER32_BASE, true);
            MAP_Interrupt_enableMaster();
    		break;
    	case ADC_READ:
    		MAP_Interrupt_disableMaster();
    		//y=0.5657x-0.1766

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

			state = DEFAULT;
		    MAP_Interrupt_enableMaster();
    		break;
    	case SEND_DATA:

    		runAsClient();

    		state = DEFAULT;
            MAP_Timer32_setCount(TIMER32_BASE,48000000);
            MAP_Timer32_enableInterrupt(TIMER32_BASE);
            MAP_Timer32_startTimer(TIMER32_BASE, true);
    	default:
    		if(MAP_GPIO_getInputPinValue(IN_CONTACT_PORT, IN_CONTACT_PIN) != inputStatus)
    		{
    			inputStatus ^= 1;
    			switch(inputStatus){
    			case INPUT_ON:
    				setColor(COLOR_16_GREEN);
    				fillCircle(IN_DOT_X, IN_DOT_Y, 8);
    				setColor(COLOR_16_WHITE);
    				break;
    			case INPUT_OFF:
    				setColor(COLOR_16_RED);
    				fillCircle(IN_DOT_X, IN_DOT_Y, 8);
    				setColor(COLOR_16_WHITE);
    				break;
    			default:
    				break;
    			}
    		}
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
        state = ADC_READ;
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

    if(status & GPIO_PIN6) adc_flag = 0;
    else if(status & GPIO_PIN7) adc_flag = 1;
	MAP_ADC14_toggleConversionTrigger();
	_delay_cycles(10000);
    MAP_Interrupt_enableMaster();
}

void PORT3_IRQHandler(void)
{
	MAP_Interrupt_disableMaster();
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    if(status & GPIO_PIN0) adc_flag = 2;
    else if(status & GPIO_PIN1) adc_flag = 3;
    else if(status & GPIO_PIN2) adc_flag = 4;
    else if(status & GPIO_PIN3) adc_flag = 5;
    else if(status & GPIO_PIN4) adc_flag = 6;
    else if(status & GPIO_PIN5) adc_flag = 7;
    else if(status & GPIO_PIN6) adc_flag = 8;
    else if(status & GPIO_PIN7) adc_flag = 9;
    MAP_ADC14_toggleConversionTrigger();
    _delay_cycles(10000);
    MAP_Interrupt_enableMaster();
}

void T32_INT1_IRQHandler(void)
{
	MAP_Interrupt_disableMaster();
	static uint16_t temp_count = 0;
	static uint16_t client_count = 0;
	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	temp_count++;
	client_count++;
	if (temp_count == 60)
	{
		state = TEMP_READ;
		temp_count = 0;
	}
	if (client_count == 16)
	{
		state = SEND_DATA;
		client_count = 0;
	}
	else MAP_Timer32_setCount(TIMER32_BASE,48000000);
	MAP_Interrupt_enableMaster();
}

