/*
 * Serial.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: taivb
 */

#include <include.h>
#include <stm32f1xx_it.h>
#include <Serial/Serial.h>

//USART_TypeDef *HardwareSerial::USARTnum;

HardwareSerial::HardwareSerial(int USART)
{

	switch (USART)
	{
		case 1:
			USARTnum = USART1;

			USART_RCC_PER = RCC_APB2Periph_USART1;

			USART_RCC_GPIO = USART1_RCC_GPIO;
			USART_RCC_AFIO = USART1_RCC_AFIO;
			USART_GPIO = USART1_GPIO;

			USART_Pin_Tx = getDigitalPin(USART1_PIN_TX);
			USART_Pin_Rx = getDigitalPin(USART1_PIN_RX);
			break;

		case 2:
			USARTnum = USART2;

			USART_RCC_PER = RCC_APB1Periph_USART2;

			USART_RCC_GPIO = USART2_RCC_GPIO;
			USART_RCC_AFIO = USART2_RCC_AFIO;
			USART_GPIO = USART2_GPIO;

			USART_Pin_Tx = getDigitalPin(USART2_PIN_TX);
			USART_Pin_Rx = getDigitalPin(USART2_PIN_RX);
			break;

		case 3:
			USARTnum = USART3;

			USART_RCC_PER = RCC_APB1Periph_USART3;

			USART_RCC_GPIO = USART3_RCC_GPIO;
			USART_RCC_AFIO = USART3_RCC_AFIO;
			USART_GPIO = USART3_GPIO;

			USART_Pin_Tx = getDigitalPin(USART3_PIN_TX);
			USART_Pin_Rx = getDigitalPin(USART3_PIN_RX);
			break;

		default:
			break;
	}
}

HardwareSerial::~HardwareSerial()
{
	// TODO Auto-generated destructor stub
}

/*
 * only for usart1
 * Rx - A10
 * Tx - A9
 */
void HardwareSerial::begin(uint32_t baud)
{
	USART_InitTypeDef Usart_InitStructure;
	GPIO_InitTypeDef Gpio_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// check USART is available
	assert_param(IS_USART_123_PERIPH(USARTnum));

	// set clock for GPIOA
	RCC_APB2PeriphClockCmd(USART_RCC_GPIO | USART_RCC_AFIO, ENABLE);
	// set clock for USART1
	if (USARTnum == USART1)
	{
		RCC_APB2PeriphClockCmd(USART_RCC_PER, ENABLE);
	}
	else if(USARTnum == USART2)
	{
		RCC_APB1PeriphClockCmd(USART_RCC_PER, ENABLE);
	}
	else if(USARTnum == USART3)
	{
		RCC_APB1PeriphClockCmd(USART_RCC_PER, ENABLE);
	}

	// config A9 - Tx pin
	Gpio_InitStructure.GPIO_Pin = (1<<USART_Pin_Tx);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO, &Gpio_InitStructure);

	// config A10 Rx pin
	Gpio_InitStructure.GPIO_Pin = (1<<USART_Pin_Rx);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO, &Gpio_InitStructure);

	// set USART1 parameter
	Usart_InitStructure.USART_BaudRate = baud;
	Usart_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	Usart_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_InitStructure.USART_Parity = USART_Parity_No;
	Usart_InitStructure.USART_StopBits = USART_StopBits_1;
	Usart_InitStructure.USART_WordLength = USART_WordLength_8b;

	//Init USART1
	USART_Init(USARTnum, &Usart_InitStructure);

	/* Enable the USARTx Interrupt */
	if (USARTnum == USART1)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else if (USARTnum == USART2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	else if (USARTnum == USART3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	//clear flag for the first time
	USART_ClearFlag(USARTnum, USART_FLAG_RXNE);

	// enable receive interrupt
	USART_ITConfig(USARTnum, USART_IT_RXNE, ENABLE);

	// enable USART 1
	USART_Cmd(USARTnum, ENABLE);
}

/*
 * disable hardware serial
 */
void HardwareSerial::end()
{
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTnum));
	if (USARTnum == USART1)
	{
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
	}
	else if (USARTnum == USART2)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
	}
	else if (USARTnum == USART3)
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
	}
}

size_t HardwareSerial::write(uint8_t val)
{
	while(USART_GetFlagStatus(USARTnum,USART_FLAG_TXE) == 0);
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTnum));
	assert_param(IS_USART_DATA(val));

	/* Transmit Data */
	USARTnum->DR = (val & (uint16_t) 0x01FF);
	return 1;
}

/*
 * receive data
 */
uint8_t HardwareSerial::read(void)
{
	// check the parameter
	assert_param(IS_USART_123_PERIPH(USARTnum));
	// wait for Receive line is free
	//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	// return value
	return USARTnum->DR & 0xff;
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if (USART_ReceiveData(USART1) == 13)// ki tu ket thuc
		{
			return;
		}
		SerialEvent();
	}
}

void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		if (USART_ReceiveData(USART2) == 13)// ki tu ket thuc
		{
			return;
		}
		Serial2Event();
	}
}

void USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if (USART_ReceiveData(USART3) == 13)// ki tu ket thuc
		{
			return;
		}
		Serial3Event();
	}
}

HardwareSerial Serial = HardwareSerial();
