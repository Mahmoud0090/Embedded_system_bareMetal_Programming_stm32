
#include "uart.h"

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define CR1_TE		(1U<<3)
#define CR1_RE		(1U<<2)

#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)
#define CR1_RXNETE	(1U<<5)

#define DMA1EN					(1U<<21)
#define CH4SEL					(1U<<27) //3 bits 25,26,27 with values 100
#define DMA_MEM_INC				(1U<<10)
#define DMA_DIR_MEM_TO_PERIPH	(1U<<6)
#define DMA_CR_TCIE				(1U<<4)
#define DMA_CR_EN				(1U<<0)
#define UART_CR3_DMAT			(1U<<7)

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


void dma1_stream6_init(uint32_t src , uint32_t dst , uint32_t len)
{
	/*Enable clock access to DMA*/
	RCC->AHB1ENR |= DMA1EN;
	/*DIsable DMA1 stream6*/
	DMA1_Stream6->CR &= ~DMA_CR_EN;

	/*wait until DMA1 stream6 is disabled*/
	while(DMA1_Stream6->CR & DMA_CR_EN){}
	/*clear all interrupt flags of stream6*/
	DMA1->HIFCR |= (1U<<16);
	DMA1->HIFCR |= (1U<<18);
	DMA1->HIFCR |= (1U<<19);
	DMA1->HIFCR |= (1U<<20);
	DMA1->HIFCR |= (1U<<21);
	/*set the destination buffer*/
	DMA1_Stream6->PAR = dst;
	/*set the source buffer*/
	DMA1_Stream6->M0AR = src;
	/*set length*/
	DMA1_Stream6->NDTR = len;
	/*select CH4 of stream6*/
	DMA1_Stream6->CR = CH4SEL;

	/*Enable memory increment*/
	DMA1_Stream6->CR |= DMA_MEM_INC;
	/*configure transfer direction */
	DMA1_Stream6->CR |= DMA_DIR_MEM_TO_PERIPH;
	/*Enable DMA transfer complete interrupt*/
	DMA1_Stream6->CR |= DMA_CR_TCIE;
	/*Enable direct mode and disable FIFO*/
	DMA1_Stream6->FCR = 0;
	/*Enable DMA1 stream6*/
	DMA1_Stream6->CR |= DMA_CR_EN;
	/*Enable UART2 transmitter DMA*/
	USART2->CR3 |= UART_CR3_DMAT;
	/*Enable DMA interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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

void uart2_rx_interrupt_init(void)
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


	/*set PA3 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	/*set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/******************configure uart module************/

	/*enable clock to uart*/
	RCC->APB1ENR |= UART2EN;

	/*configure baudrate*/
	uart_set_baudrate(USART2 , APB1_CLK , UART_BAUDRATE);

	/*configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE); // enable tx and rx

	/*Enable RXNE interrupt*/
	USART2->CR1 |= CR1_RXNETE;

	/*enable interrupt in NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);

	/*enable uart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init(void)
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


	/*set PA3 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	/*set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/******************configure uart module************/

	/*enable clock to uart*/
	RCC->APB1ENR |= UART2EN;

	/*configure baudrate*/
	uart_set_baudrate(USART2 , APB1_CLK , UART_BAUDRATE);

	/*configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE); // enable tx and rx

	/*enable uart module*/
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void)
{
	/*make sure receive data register is not empty*/
	while(!(USART2->SR & SR_RXNE)){}

	/*read data*/
	return USART2->DR;
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
