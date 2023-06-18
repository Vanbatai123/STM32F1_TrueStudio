/*
* UART.c
*
* Created: 10/27/2018 10:17:47 AM
*  Author: PC
*/

#include <include.h>
#include "uart3.h"
#include <stm32f1xx_it.h>


void UART3_config(uint32_t baudrate)
{

	USART_InitTypeDef Usart_InitStructure;
	GPIO_InitTypeDef Gpio_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// check USART is available
	assert_param(IS_USART_123_PERIPH(USART3));

	// set clock for GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	// set clock for USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// config A2 - Tx pin
	Gpio_InitStructure.GPIO_Pin = (1 << 10);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Gpio_InitStructure);

	// config A3 Rx pin
	Gpio_InitStructure.GPIO_Pin = (1 << 11);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &Gpio_InitStructure);

	// set USART3 parameter
	Usart_InitStructure.USART_BaudRate = baudrate;
	Usart_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	Usart_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_InitStructure.USART_Parity = USART_Parity_No;
	Usart_InitStructure.USART_StopBits = USART_StopBits_1;
	Usart_InitStructure.USART_WordLength = USART_WordLength_8b;

	//Init USART3
	USART_Init(USART3, &Usart_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//clear flag for the first time
	USART_ClearFlag(USART3, USART_FLAG_RXNE);

	// enable receive interrupt
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	// enable USART 1
	USART_Cmd(USART3, ENABLE);
}

void UART3_putChar(char c)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == 0);
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USART3));
	assert_param(IS_USART_DATA(c));

	/* Transmit Data */
	USART3 -> DR = (c & (uint16_t) 0x01FF);
}

void UART3_print(char* str)
{
	while (*str)// loop through entire string
	{
		UART3_putChar(*str);
		str++;
	}
}

void UART3_printNum(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li", num);
	else if (base == HEX)	sprintf(dis, "0x%lx", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number");
	UART3_print(dis);
}

void UART3_printNumln(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li\r\n", num);
	else if (base == HEX)	sprintf(dis, "0x%lx\r\n", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number\r\n");
	UART3_print(dis);
}

void UART3_printFloat(double num, uint8_t digits)
{
	if (isnan(num)) return UART3_print("nan");
	if (isinf(num)) return UART3_print("inf");
	if (num > 4294967040.0) return UART3_print ("ovf");  // constant determined empirically
	if (num <-4294967040.0) return UART3_print ("ovf");  // constant determined empirically

	// Handle negative numbers
	if (num < 0.0)
	{
		UART3_putChar('-');
		num = -num;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;

	num += rounding;

	// Extract the integer part of the num and print it
	unsigned long int_part = (unsigned long)num;
	double remainder = num - (double)int_part;
	UART3_printNum(int_part, DEC);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		UART3_putChar('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		UART3_printNum(toPrint, DEC);
		remainder -= toPrint;
	}
}

void UART3_printFloatln(double num, uint8_t digits)
{
	UART3_printFloat(num, digits);
	UART3_print("\r\n");
}

void UART3_println(char* str)
{
	UART3_print(str);
	UART3_print("\r\n");
}

uint8_t UART3_getChar()
{
	// check the parameter
	assert_param(IS_USART_123_PERIPH(USART3));
	// wait for Receive line is free
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
	// return value
	return USART3->DR & 0xff;
}

void USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if (USART_ReceiveData(USART3) == 13)// ki tu ket thuc
		{
			return;
		}
		UART3_Event();
	}
}
