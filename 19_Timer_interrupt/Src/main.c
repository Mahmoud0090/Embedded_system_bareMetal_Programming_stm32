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

static void tim2_callback(void);

int main(void)
{
	//1-enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//2-set PA5 as output pin
	GPIOA->MODER |= (1<<10);
	GPIOA->MODER &= ~(1<<11);

	uart2_tx_init();
	tim2_1Hz_interrupt_init();

	while(1)
	{



	}
}

static void tim2_callback(void)
{
	printf("A second passed !! \n\r");
	GPIOA->ODR ^= LED_PIN;
}

void TIM2_IRQHandler(void)
{
	/*clear UIF*/
	TIM2->SR &= ~SR_UIF;

	/*do something*/
	tim2_callback();
}


