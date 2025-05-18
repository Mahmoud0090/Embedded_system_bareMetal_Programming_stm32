#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)

#define LED_PIN		PIN5

int main(void)
{
	//1-enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//2-set PA5 as output pin
	GPIOA->MODER |= (1<<10);
	GPIOA->MODER &= ~(1<<11);

	uart2_tx_init();
	tim2_1Hz_init();

	while(1)
	{

		/*wait for UIF*/
		while(!(TIM2->SR & SR_UIF)){};

		/*clear UIF*/
		TIM2->SR &= ~SR_UIF;

		printf("A second passed !! \n\r");
		GPIOA->ODR ^= LED_PIN;


	}
}


