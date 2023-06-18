/*
 * include.c
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#include "include.h"

uint32_t tim_cnt = 0;
uint32_t ticker;
volatile uint32_t us;

void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000)) //1ms per interrupt
	{
		while(1);
	}
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_SetPriority(SysTick_IRQn, 0);
	ticker = 0;
}

uint32_t millis(void)
{
	return ticker;
}

uint32_t micros(void)
{
	us = ticker*1000+(SystemCoreClock/1000-SysTick->VAL)/80;
//	us = ticker * 1000 + 1000 - SysTick->VAL / 80;
	return us;
}

void _delay_ms(uint32_t nTime)
{
	uint32_t curTime = ticker;
	while ((nTime - (ticker - curTime)) > 0);
}

void _delay_us(uint32_t nTime)
{
	uint32_t curTime = micros();
	while((nTime-(micros()-curTime)) > 0);
}


void SysTick_Handler(void)
{
	ticker++;
}

void randomSeed(unsigned long seed)
{
  if (seed != 0) {
    srandom(seed);
  }
}

long random1(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return random() % howbig;
}

long random2(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random1(diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
