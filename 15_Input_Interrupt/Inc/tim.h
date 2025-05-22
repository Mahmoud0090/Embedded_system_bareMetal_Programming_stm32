/*
 * tim.h
 *
 *  Created on: May 18, 2025
 *      Author: User
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f4xx.h"

void tim2_1Hz_init(void);
void tim2_pa5_output_compare(void);
void tim3_pa6_input_capture(void);


#define SR_UIF		(1U<<0)
#define SR_CC1If	(1U<<1)
#endif /* TIM_H_ */
