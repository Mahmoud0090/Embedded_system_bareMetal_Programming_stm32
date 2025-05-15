#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	uart2_tx_init();
	pa1_adc_init();

	start_conversion(); // no need to put this func inside
						//while loop since continuous conversion
						//is enabled

	while(1)
	{
		sensor_value = adc_read();
		printf("sensor value :  %d\n\r" , (int)sensor_value);
	}
}


