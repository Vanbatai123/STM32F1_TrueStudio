/*
 * include_pins.h
 *
 *  Created on: Sep 25, 2018
 *      Author: PC
 */

#ifndef INCLUDE_PINS_H_
#define INCLUDE_PINS_H_


// GPIO pin definition
#define	A0		0
#define	A1		1
#define	A2		2
#define	A3		3
#define	A4		4
#define	A5		5
#define	A6		6
#define	A7		7
#define	A8		8
#define	A9		9
#define	A10		10
#define	A11		11
#define	A12		12
#define	A13		13
#define	A14		14
#define	A15		15

#define	B0		16
#define	B1		17
#define	B2		18
#define	B3		19
#define	B4		20
#define	B5		21
#define	B6		22
#define	B7		23
#define	B8		24
#define	B9		25
#define	B10		26
#define	B11		27
#define	B12		28
#define	B13		29
#define	B14		30
#define	B15		31

#define	C0		32
#define	C1		33
#define	C2		34
#define	C3		35
#define	C4		36
#define	C5		37
#define	C6		38
#define	C7		39
#define	C8		40
#define	C9		41
#define	C10		42
#define	C11		43
#define	C12		44
#define	C13		45
#define	C14		46
#define	C15		47


// USART pins definition
#define USART1_GPIO			GPIOA
#define USART1_RCC_GPIO 	RCC_APB2Periph_GPIOA
#define USART1_RCC_AFIO		RCC_APB2Periph_AFIO
#define USART1_PIN_TX		A9
#define USART1_PIN_RX		A10

#define USART2_GPIO			GPIOA
#define USART2_RCC_GPIO 	RCC_APB2Periph_GPIOA
#define USART2_RCC_AFIO		RCC_APB2Periph_AFIO
#define USART2_PIN_TX		A2
#define USART2_PIN_RX		A3

#define USART3_GPIO			GPIOB
#define USART3_RCC_GPIO 	RCC_APB2Periph_GPIOB
#define USART3_RCC_AFIO		RCC_APB2Periph_AFIO
#define USART3_PIN_TX		B10
#define USART3_PIN_RX		B11


// I2C pins definition
#define I2C1_GPIO			GPIOB
#define I2C1_RCC_GPIO 		RCC_APB2Periph_GPIOB
#define I2C1_RCC_AFIO		RCC_APB2Periph_AFIO
#define I2C1_PIN_SCL		GPIO_Pin_6
#define I2C1_PIN_SDA		GPIO_Pin_7


// SPI pins definition

// SPI1 define PIN/GPIO
#define SPI1_GPIO			GPIOA
#define SPI1_RCC_GPIO 		RCC_APB2Periph_GPIOA
#define SPI1_RCC_AFIO		RCC_APB2Periph_AFIO

#define SPI1_MODE_MASTER	0x00
#define SPI1_MODE_SLAVE		0x01

#define SPI1_PIN_NSS		A4
#define SPI1_PIN_SCK		A5
#define SPI1_PIN_MISO		A6
#define SPI1_PIN_MOSI		A7

// SPI2 define PIN/GPIO
#define SPI2_GPIO			GPIOB
#define SPI2_RCC_GPIO 		RCC_APB2Periph_GPIOB
#define SPI2_RCC_AFIO		RCC_APB2Periph_AFIO

#define SPI2_MODE_MASTER	0x00
#define SPI2_MODE_SLAVE		0x01

#define SPI2_PIN_NSS		B12
#define SPI2_PIN_SCK		B13
#define SPI2_PIN_MISO		B14
#define SPI2_PIN_MOSI		B15

/// only for SPI1
#define SS 					SPI2_PIN_NSS
#define SCK 				SPI2_PIN_SCK
#define MISO 				SPI2_PIN_MISO
#define MOSI 				SPI2_PIN_MOSI

#endif /* INCLUDE_PINS_H_ */
