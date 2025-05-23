#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)

#define LED			PIN5

/*set up : connect jumper wire from PA5 to PA6*/
int main(void)
{
	//enable clock to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//set PA5 as output pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);


	pc13_exti_init();
	uart2_tx_init();

	while(1)
	{

	}
}

static void exti_callback(void)
{
	printf("button pressed\n\r");
	GPIOA->ODR ^= LED;

}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & LINE13) != 0)
	{
		//clear PR flag
		EXTI->PR |= LINE13;
		//do something
		exti_callback();
	}
}
