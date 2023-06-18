/*
 * include.c
 *
 *  Created on: Aug 8, 2018
 *      Author: taivb
 */

#include "include.h"

uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

void delay(__IO uint32_t ms)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}

	TimingDelay = ms;
	while (TimingDelay != 0);
}
