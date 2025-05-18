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

#define SR_UIF	(1U<<0)

#endif /* TIM_H_ */
