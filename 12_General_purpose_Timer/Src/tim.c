#include "tim.h"


#define TIM2EN	(1U<<0)
#define CR1_CEN	(1U<<0)

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


