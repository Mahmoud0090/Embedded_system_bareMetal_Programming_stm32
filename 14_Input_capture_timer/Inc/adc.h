/*
 * adc.h
 *
 *  Created on: May 14, 2025
 *      Author: User
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"
#include <stdint.h>
void pa1_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
