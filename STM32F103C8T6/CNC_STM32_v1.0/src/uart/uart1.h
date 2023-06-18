/*
* UART1.h
*
* Created: 11/8/2018 10:23:05 AM
*  Author: Van_BasTai
*/


#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>

void UART1_config(uint32_t baudrate);
void UART1_putChar(char c);
void UART1_print(char* str);
void UART1_printNum(long num, uint8_t base);
void UART1_printNumln(long num, uint8_t base);
void UART1_printFloat(double num, uint8_t digits);
void UART1_printFloatln(double num, uint8_t digits);
void UART1_println(char* str);
uint8_t UART1_getChar();


#endif /* UART_H_ */
