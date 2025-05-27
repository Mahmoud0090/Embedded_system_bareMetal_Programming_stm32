#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)

#define LED_PIN		PIN5

static void systick_callback(void);

int main(void)
{
	//1-enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//2-set PA5 as output pin
	GPIOA->MODER |= (1<<10);
	GPIOA->MODER &= ~(1<<11);

	uart2_tx_init();
	systick_1hz_interrupt();

	while(1)
	{

	}
}

static void systick_callback(void)
{
	printf("A second passed !! \n\r");
	GPIOA->ODR ^= LED_PIN;
}

void SysTick_Handler(void)
{
	systick_callback();
}
