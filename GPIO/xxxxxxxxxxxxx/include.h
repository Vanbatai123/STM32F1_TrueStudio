/*
 * include.h
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include "stm32f10x.h"
#include "stm32f1xx_it.h"
#include "stm32f10x_tim.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
//#include <Ethernet/WString.h>

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

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*voidFuncPtr)(void);
typedef uint8_t byte;
typedef bool boolean;
void TimingDelay_Decrement(void);
void delay(__IO uint32_t i);
void delayMicroseconds(__IO uint32_t i);
uint32_t millis();
uint32_t micros();
void millisInit();
void Timer2Init();
void xxx(uint32_t);

#include <stm32f10x.h>
#include <misc.h>

void SysTick_Init(void);
void delay_ms(uint32_t nTime);
void delay_us(uint32_t nTime);
uint32_t generated_ms(void);
uint32_t generated_us(void);
#ifdef __cplusplus
} // extern "C"
#endif

#include <GPIO/GPIO.h>
#include <Interrupt/Interrupt.h>

#endif /* INCLUDE_H_ */
