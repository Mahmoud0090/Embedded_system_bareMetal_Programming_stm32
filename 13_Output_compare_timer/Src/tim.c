#include "tim.h"


#define TIM2EN		(1U<<0)
#define CR1_CEN		(1U<<0)
#define OC_TOGGLE	(1U<<4) | (1U<<5)
#define CCER_CC1E	(1U<<0)

#define GPIOAEN		(1U<<0)
#define AFR5_TIM	(1U<<20) // this 0001 (for bits 20,21,22,23)

void tim2_1Hz_init(void)
{
	/*enable clock access to tim2*/
	RCC->APB1ENR |= TIM2EN;
	/*set prescaler value*/
	TIM2->PSC = 1600 - 1; // 16 000 000 / 1600 = 10 000
	/*set Auto_reload value*/
	TIM2->ARR = 10000 - 1; // for 1hz
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 = CR1_CEN;
}

void tim2_pa5_output_compare(void)
{
	/*enable clock to gpioa*/
	RCC->AHB1ENR |= GPIOAEN;
	/*set PA5 mode to alternative mode*/
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11);
	/*set PA5 alternative function to TIM2_CH1*/
	GPIOA->AFR[0] |= AFR5_TIM;
	/*enable clock access to tim2*/
	RCC->APB1ENR |= TIM2EN;
	/*set prescaler value*/
	TIM2->PSC = 1600 - 1; // 16 000 000 / 1600 = 10 000
	/*set Auto_reload value*/
	TIM2->ARR = 10000 - 1; // for 1hz

	/*set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE; // set bits to 011(for ch1)

	/*enable timer2 channel 1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 = CR1_CEN;
}
