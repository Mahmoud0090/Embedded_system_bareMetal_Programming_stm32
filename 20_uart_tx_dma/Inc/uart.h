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
void dma1_stream6_init(uint32_t src , uint32_t dst , uint32_t len);

#define SR_RXNE				(1U<<5) //receive data reg not empty (0 nothing to read , 1 ready to read)
#define HISR_TCIF6			(1U<<21)
#define HIFCR_CTCIF6		(1U<<21)
#endif /* UART_H_ */
