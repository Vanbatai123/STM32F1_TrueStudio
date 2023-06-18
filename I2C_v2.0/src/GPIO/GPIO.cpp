/*
 * GPIO.c
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */



#include <include.h>
#include <GPIO/GPIO.h>

/*
 * pin configuration function
 */
void pinMode(GPIO_TypeDef *gpio, uint16_t pin, uint8_t mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_AHPclock Clock */
	if(gpio == GPIOA)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	if(gpio == GPIOB)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	if(gpio == GPIOC)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	if(gpio == GPIOD)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	if(gpio == GPIOE)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	if(gpio == GPIOH)	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

	/* Configure the GPIO_LED pin */
	switch (mode)
	{
		case OUTPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			break;

		case INPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			break;

		case INPUT_PULLUP:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			break;

		case INPUT_PULLDOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
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
void digitalWrite(GPIO_TypeDef* gpio, uint16_t pin, uint8_t val)
{
	if (val == HIGH)	gpio->BSRRL = pin;
	else	gpio->BSRRH = pin;
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
