/*
* UART3.h
*
* Created: 11/8/2018 10:23:05 AM
*  Author: Van_BasTai
*/


#ifndef UART3_H_
#define UART3_H_

#include <stdint.h>
//#define USE_PRINTF


void UART3_config(uint32_t baudrate);
void UART3_putChar(char c);
void UART3_print(char* str);
void UART3_printNum(int num);
void UART3_printNumln(int num);
void UART3_printFloat(double num, uint8_t digits);
void UART3_printFloatln(double num, uint8_t digits);
void UART3_println(char* str);
uint8_t UART3_getChar();
void UART3_printXY(float x, float y);
// interrupt handler
extern void UART3_Event(void) __attribute__((weak));

#endif /* UART_H_ */
