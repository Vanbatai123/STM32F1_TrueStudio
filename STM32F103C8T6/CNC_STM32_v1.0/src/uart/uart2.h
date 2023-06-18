/*
* UART2.h
*
* Created: 11/8/2018 10:23:05 AM
*  Author: Van_BasTai
*/


#ifndef UART2_H_
#define UART2_H_

#include <stdint.h>
//#define USE_PRINTF


void UART2_config(uint32_t baudrate);
void UART2_putChar(char c);
void UART2_print(char* str);
void UART2_printNum(long num, uint8_t base);
void UART2_printNumln(long num, uint8_t base);
void UART2_printFloat(double num, uint8_t digits);
void UART2_printFloatln(double num, uint8_t digits);
void UART2_println(char* str);
uint8_t UART2_getChar();


#endif /* UART_H_ */
