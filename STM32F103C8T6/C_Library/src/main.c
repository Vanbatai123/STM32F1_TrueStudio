/*
 * Project STM32F103C8T6 C_library
 *
 *	Done: SPI1, USART1
 *
 *  Created on: Nov 8, 2018
 *      Author: Van_BasTai
 */

/* include */
#include <include.h>

/* Private define */

/* Private typedef */

/* Private function prototypes */

/* Private functions */

/* Private variable */

/************************************************************/
/*                  MAIN FUNCTION							*/
/************************************************************/
int main(void)
{
	SysTick_Init();
	pinMode(C13, OUTPUT);
	UART3_config(115200);
	UART3_println("van ba tai OK!!!");
	while (1)
	{
		daobit(GPIOC, 13);
		delay(1000);
	}
}
void Serial3Event()
{
	UART3_putChar((char)USART_ReceiveData(USART3));
}
