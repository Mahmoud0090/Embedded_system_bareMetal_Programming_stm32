#include "systick.h"

#define SYSTICK_LOAD_VALUE		16000 // for 1ms delay 16000/16MHZ = 1ms
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<2) //1 for processor clock , 0 for external clock
#define CTRL_COUNTFLAG			(1U<<16) // return 1 if timer counted to 0

void SystickDelayMs(int delay)
{
	/*reload with number of clock cycles per milliseconds*/
	SysTick->LOAD = SYSTICK_LOAD_VALUE;

	/*clear systick current value register*/
	SysTick->VAL = 0;

	/*enable systick and select internal clk source*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i<delay ; i++)
	{
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){};
	}

	SysTick->CTRL = 0;
}
