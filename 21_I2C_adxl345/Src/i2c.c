#include "i2c.h"



#define GPIOBEN		(1U<<1)
#define I2CEN		(1U<<21)
#define I2CEN		(1U<<21)

#define I2C_100KHZ				80 //0B 0101 0000 decimal=80
#define SD_MODE_MAX_RISE_TIME	17
#define CR1_PE		(1U<<0)

/** pinout
 * PB8 ------ SCL
 * PB9 ------ SDA
 * **/

void I2C1_init(void)
{
	/*enable clock access to GPIOB*/
	RCC->AHB1ENR |= GPIOBEN;
	/*set PB8 and PB9 mode to alternate function*/
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);
	/*set PB8 and PB9 output type to open drain*/
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);
	/*enable pull-up for PB8 and PB9*/
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &= ~(1U<<19);

	/*enable clock access to I2C1*/
	RCC->AHB1ENR |= I2CEN;

	/*enter reset mode*/
	I2C1->CR1 |= (1U<<15);

	/*come out of reset mode*/
	I2C1->CR1 &= ~(1U<<15);

	/*set peripheral clock frequency*/
	I2C1->CR2 |= (1U<<4); //16MHz

	/*set I2C to standard mode, 100khz clock*/
	I2C1->CCR = I2C_100KHZ;

	/*set rise time*/
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/*enable I2C1 module*/
	I2C1->CR1 |= CR1_PE;
}
