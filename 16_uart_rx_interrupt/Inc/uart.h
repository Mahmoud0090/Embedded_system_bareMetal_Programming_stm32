/*
 * uart.h
 *
 *  Created on: May 14, 2025
 *      Author: User
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);

#define SR_RXNE		(1U<<5) //receive data reg not empty (0 nothing to read , 1 ready to read)


#endif /* UART_H_ */
