/*
 * Serial.h
 *
 *  Created on: Aug 15, 2018
 *      Author: taivb
 */

#ifndef SERIAL_SERIAL_H_
#define SERIAL_SERIAL_H_

#include <stddef.h>
#include <stdint.h>
#include <Print.h>


class HardwareSerial : public Print
{
public:
	HardwareSerial(int USART = 1);
	virtual ~HardwareSerial();
	void begin(uint32_t baud);
	void begin(USART_TypeDef *USARTx, uint32_t baud);
	void end();
	uint8_t read(void);
	size_t write(uint8_t val);
private:
	USART_TypeDef* USARTnum;

	uint32_t USART_RCC_PER;
	uint32_t USART_RCC_GPIO;
	uint32_t USART_RCC_AFIO;
	GPIO_TypeDef *USART_GPIO;

	uint8_t USART_Pin_Tx;
	uint8_t USART_Pin_Rx;
};

// interrupt handler
extern void SerialEvent(void) __attribute__((weak));
extern void Serial2Event(void) __attribute__((weak));
extern void Serial3Event(void) __attribute__((weak));

extern HardwareSerial Serial;
#endif /* SERIAL_SERIAL_H_ */
