
#include "uart.h"

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define CR1_TE		(1U<<3)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define SYS_FREQ	16000000 //16Mhz the default HSI clock frequency
#define APB1_CLK	SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef* USARTX, uint32_t PeriphClk,uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate);


void uart2_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_tx_init(void)
{
	/******************configure uart gpio pin************/
	/*enable clock to gpioa*/
	RCC->AHB1ENR |= GPIOAEN;
	/*set PA2 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	/*set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/******************configure uart module************/

	/*enable clock to uart*/
	RCC->APB1ENR |= UART2EN;

	/*configure baudrate*/
	uart_set_baudrate(USART2 , APB1_CLK , UART_BAUDRATE);

	/*configure the transfer direction*/
	USART2->CR1 = CR1_TE;

	/*enable uart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	/*make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)){}
	/*write to transmit data register*/
	USART2->DR = (ch & 0xFF); //8 bit data
}

static void uart_set_baudrate(USART_TypeDef* USARTX, uint32_t PeriphClk,uint32_t BaudRate)
{
	USARTX->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
