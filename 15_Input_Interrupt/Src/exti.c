#include "exti.h"

#define GPIOCEN		(1U<<2)
#define SYSCFGEN	(1U<<14)

void pc13_exti_init(void)
{
	/*disable global interrupts*/
	__disable_irq();
	/*enable clock to GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;

	/*set PC13 as input*/
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	/*enable clock to SYSCFG*/
	RCC->APB2ENR |= SYSCFGEN;
	/*select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1U<<5);

	/*unmask EXTI13*/
	EXTI->IMR |= (1U<<13);
	/*select falling edge trigger*/
	EXTI->FTSR |= (1U<<13);

	/*enable EXTI13 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	/*enable global interrupts*/
	__enable_irq();
}

