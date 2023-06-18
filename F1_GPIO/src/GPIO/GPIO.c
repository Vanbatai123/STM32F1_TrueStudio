/*
 * GPIO.c
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */


#include "../include.h"
#include "GPIO.h"

#define  STM32F103C8T6

#ifdef STM32F103C8T6
/*
 * pin configuration function
 */
void pinMode(GPIO_TypeDef *gpio, uint16_t pin, uint8_t mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_AHPclock Clock */
	if(gpio == GPIOA)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(gpio == GPIOB)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(gpio == GPIOC)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(gpio == GPIOD)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(gpio == GPIOE)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(gpio == GPIOF)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(gpio == GPIOG)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

	/* Configure the GPIO_LED pin */
	switch (mode)
	{
		case OUTPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			break;

		case INPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			break;

		case INPUT_PULLUP:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			break;

		case INPUT_PULLDOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			break;
		default:
			break;
	}

	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_Init(gpio, &GPIO_InitStructure);
}// end pinMode function


/*
 * write pin function
 */
void digitalWrite(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t val)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	if (val == HIGH)
		GPIOx->BSRR = pin;
	else
		GPIOx->BRR = pin;
}

/*
 * pin toggle function
 */
void digitalToggle(GPIO_TypeDef* gpio, uint16_t pin)
{
	gpio->ODR ^= pin;
}

/*
 * pin read function
 */
uint8_t digitalRead(GPIO_TypeDef* gpio, uint16_t pin)
{
  return GPIO_ReadInputDataBit(gpio, pin);
}
#endif
