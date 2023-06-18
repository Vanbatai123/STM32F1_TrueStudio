/*
* UART.c
*
* Created: 10/27/2018 10:17:47 AM
*  Author: PC
*/

#include "uart2.h"
#include <include.h>


void UART2_config(uint32_t baudrate)
{

	USART_InitTypeDef Usart_InitStructure;
	GPIO_InitTypeDef Gpio_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// check USART is available
	assert_param(IS_USART_123_PERIPH(USART2));

	// set clock for GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	// set clock for USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// config A2 - Tx pin
	Gpio_InitStructure.GPIO_Pin = (1 << 2);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_InitStructure);

	// config A3 Rx pin
	Gpio_InitStructure.GPIO_Pin = (1 << 3);
	Gpio_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &Gpio_InitStructure);

	// set USART2 parameter
	Usart_InitStructure.USART_BaudRate = baudrate;
	Usart_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	Usart_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_InitStructure.USART_Parity = USART_Parity_No;
	Usart_InitStructure.USART_StopBits = USART_StopBits_1;
	Usart_InitStructure.USART_WordLength = USART_WordLength_8b;

	//Init USART2
	USART_Init(USART2, &Usart_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//clear flag for the first time
	USART_ClearFlag(USART2, USART_FLAG_RXNE);

	// enable receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// enable USART 1
	USART_Cmd(USART2, ENABLE);
}

void UART2_putChar(char c)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 0);
	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USART2));
	assert_param(IS_USART_DATA(c));

	/* Transmit Data */
	USART2 -> DR = (c & (uint16_t) 0x01FF);
}

void UART2_print(char* str)
{
	while (*str)// loop through entire string
	{
		UART2_putChar(*str);
		str++;
	}
}

void UART2_printNum(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li", num);
	else if (base == HEX)	sprintf(dis, "0x%lx", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number");
	UART2_print(dis);
}

void UART2_printNumln(long num, uint8_t base)
{
	char dis[20];
	if (base == DEC)	sprintf(dis, "%li\r\n", num);
	else if (base == HEX)	sprintf(dis, "0x%lx\r\n", num);
	else if (base == BIN)	sprintf(dis, "can't print binary number\r\n");
	UART2_print(dis);
}

void UART2_printFloat(double num, uint8_t digits)
{
	if (isnan(num)) return UART2_print("nan");
	if (isinf(num)) return UART2_print("inf");
	if (num > 4294967040.0) return UART2_print ("ovf");  // constant determined empirically
	if (num <-4294967040.0) return UART2_print ("ovf");  // constant determined empirically

	// Handle negative numbers
	if (num < 0.0)
	{
		UART2_putChar('-');
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
	UART2_printNum(int_part, DEC);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		UART2_putChar('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		UART2_printNum(toPrint, DEC);
		remainder -= toPrint;
	}
}

void UART2_printFloatln(double num, uint8_t digits)
{
	UART2_printFloat(num, digits);
	UART2_print("\r\n");
}

void UART2_println(char* str)
{
	UART2_print(str);
	UART2_print("\r\n");
}

uint8_t UART2_getChar()
{
	// check the parameter
	assert_param(IS_USART_123_PERIPH(USART2));
	// wait for Receive line is free
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	// return value
	return USART2->DR & 0xff;
}
