#include "stm32f4xx.h"


#define GPIOAEN		(1U<<0)
#define GPIOCEN		(1U<<2)

#define PIN5		(1U<<5)
#define PIN13		(1U<<13)

#define LED_PIN		PIN5
#define BTN_PIN		PIN13


int main(void)
{
	/*enable clock to GPIOA and GPIOC*/
	RCC->AHB1ENR |= GPIOAEN;
	RCC->AHB1ENR |= GPIOCEN;

	/*set PA5 as output pin*/
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/*set PC13 as input pin*/
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	while(1)
	{

		/*check in BTN is pressed*/
		if(GPIOC->IDR & BTN_PIN)
		{
			GPIOA->BSRR = LED_PIN;	// set pin 5
		}
		else
		GPIOA->BSRR = (1U<<21); // reset the pin5

	}
}
