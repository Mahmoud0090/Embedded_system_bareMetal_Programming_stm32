#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

int timestamp = 0;

/*set up : connect jumper wire from PA5 to PA6*/
int main(void)
{

	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1)
	{
		/*wait until edge is captured*/
		while(!(TIM3->SR & SR_CC1If)){};

		/*read value*/
		timestamp  = TIM3->CCR1;
	}
}


