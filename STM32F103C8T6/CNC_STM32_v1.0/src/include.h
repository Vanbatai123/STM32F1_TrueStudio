/*
 * include.h
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <include_pins.h>

#include "stm32f10x.h"
#include "stm32f1xx_it.h"
#include "stm32f10x_tim.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdfix.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <misc.h>



#include <GPIO/GPIO.h>
#include <uart/uart3.h>
#include "stepper/stepper.h"
#include "Interrupt/Interrupt.h"
#include "gcode/gcode.h"

#define setb(GPIO_PINx)	getDigitalGPIO(GPIO_PINx)->BSRR = (1<< getDigitalPin(GPIO_PINx))
#define clrb(GPIO_PINx)	getDigitalGPIO(GPIO_PINx)->BRR = (1<< getDigitalPin(GPIO_PINx))
#define daobit(GPIO_PINx) getDigitalGPIO(GPIO_PINx)->ODR ^= (1<< getDigitalPin(GPIO_PINx))


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abss(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN	2


typedef void (*voidFuncPtr)(void);
typedef uint8_t byte;
typedef bool boolean;
void TimingDelay_Decrement(void);
void _delay_ms(__IO uint32_t i);
void _delay_us(__IO uint32_t i);
uint32_t millis();
uint32_t micros();
void millisInit();
void Timer2Init();
void xxx(uint32_t);


void SysTick_Init(void);
uint32_t generated_ms(void);
uint32_t generated_us(void);

// WMath prototypes
long random1(long);
long random2(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

// SPI mode
#define SPI_Mode_Master                 ((uint16_t)0x0104)
#define SPI_Mode_Slave                  ((uint16_t)0x0000)


#endif /* INCLUDE_H_ */
