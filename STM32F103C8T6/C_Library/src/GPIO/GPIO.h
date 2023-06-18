/*
 * GPIO.h
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

// pin mode definition
#define INPUT				0x04
#define OUTPUT				0x10
#define INPUT_PULLUP		0x48
#define INPUT_PULLDOWN 		0x28
#define AF_PP				0x18
#define AF_OD		 		0x1C


#define LOW					0x00
#define HIGH				0x01

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

//Private function
GPIO_TypeDef *getDigitalGPIO(uint8_t PINx);
uint8_t getDigitalPin(uint8_t PINx);

// digital Function
void pinMode(uint8_t GPIO_PINx, uint8_t mode);
uint8_t digitalRead(uint8_t GPIO_PINx);

// analog Function
void analogEnable(uint8_t ADC_PINx);
void analogDisable();
uint16_t analogRead(uint8_t GPIO_PINx);

// ADC_DMA
void ADC_DMA_Init(uint16_t ADC_values[], uint8_t len);

// PWM output functions
void analogWrite(uint8_t GPIO_PINx, uint16_t val);
void PWMInit(uint8_t GPIO_PINx);



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
