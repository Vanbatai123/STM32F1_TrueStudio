/*
 * include.h
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_



#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "stm32f4xx.h"




void TimingDelay_Decrement(void);
void delay(__IO uint32_t ms);


#include "GPIO/GPIO.h"



#endif /* INCLUDE_H_ */
