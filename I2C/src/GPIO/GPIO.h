/*
 * GPIO.h
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

// pin mode definition
#define INPUT				0x00
#define OUTPUT				0x01
#define INPUT_PULLUP		0x02
#define INPUT_PULLDOWN 		0x03

#define LOW					0x00
#define HIGH				0x01

//GPIO
#define 		GA			GPIOA
#define 		GB			GPIOB
#define 		GC			GPIOC
#define 		GD			GPIOD
#define 		GE			GPIOE
#define 		GH			GPIOH

//PIN
#define 		P0			GPIO_Pin_0
#define 		P1			GPIO_Pin_1
#define 		P2			GPIO_Pin_2
#define 		P3			GPIO_Pin_3
#define 		P4			GPIO_Pin_4
#define 		P5			GPIO_Pin_5
#define 		P6			GPIO_Pin_6
#define 		P7			GPIO_Pin_7
#define 		P8			GPIO_Pin_8
#define 		P9			GPIO_Pin_9
#define 		P10			GPIO_Pin_10
#define 		P11			GPIO_Pin_11
#define 		P12			GPIO_Pin_12
#define 		P13			GPIO_Pin_13
#define 		P14			GPIO_Pin_14
#define 		P15			GPIO_Pin_15


//User function
uint8_t digitalRead(GPIO_TypeDef* gpio, uint16_t pin);
void digitalToggle( GPIO_TypeDef* gpio, uint16_t pin);
void pinMode(GPIO_TypeDef* gpio, uint16_t pin, uint8_t mode);
void digitalWrite( GPIO_TypeDef* gpio, uint16_t pin, uint8_t val);



// Library function
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);



#endif /* GPIO_GPIO_H_ */
