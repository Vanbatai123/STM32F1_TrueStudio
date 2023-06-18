/*
 *
 * Project convert STM32 to Arduino
 *
 *  Created on: Aug 8, 2018
 *      Author: Van_BasTai
 */


#include <include.h>
#include <i2c.c>
//#include <../Libraries/w>


/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define I2C1_DR_Address        0x40005410
#define I2C2_DR_Address        0x40005810
#define I2C1_SLAVE_ADDRESS7    0x68
#define I2C2_SLAVE_ADDRESS7    0x68
#define BufferSize             7
#define ClockSpeed             30000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;
uint8_t I2C1_Buffer_Tx[BufferSize] = {1, 2, 3, 4, 5, 6, 7};
uint8_t I2C2_Buffer_Rx[BufferSize];
uint8_t Tx_Idx = 0, Rx_Idx = 0;
volatile TestStatus TransferStatus;

uint8_t qq=10;

/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
void _delay(__IO uint32_t ms)
{
	while(ms--)
	{
		int x = 16000;
		while(x--);
	}
}
uint8_t fff[7];
int main(void)
{
	I2C_LowLevel_Init(I2C1, 30000, 0xEE);

	while (1)
	{
		I2C_Read(I2C1,fff,7,0xD0);
		qq = fff[0];
		Tx_Idx++;
		_delay(500);
	}
}
