/*
 * include.h
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_


#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "stm32f10x.h"



#ifdef __cplusplus
extern "C"{
#endif
void SysTick_Handler(void);
void delayMicroseconds(__IO uint32_t i);
void delay(__IO uint32_t i);

#ifdef __cplusplus
} // extern "C"
#endif


#include <GPIO/GPIO.h>


#endif /* INCLUDE_H_ */
