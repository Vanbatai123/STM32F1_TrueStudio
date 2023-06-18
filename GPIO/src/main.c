/*
 * Wire.cpp
 *
 * Project convert STM32 to Arduino
 *  Created on: Aug 8, 2018
 *      Author: Van_BasTai
 */


#include <include.h>
#include <Wire/Wire.h>


/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define I2C1_DR_Address        0x40005410
#define I2C2_DR_Address        0x40005810
#define I2C1_SLAVE_ADDRESS7    0x68
#define I2C2_SLAVE_ADDRESS7    0x68
#define BufferSize             8
#define ClockSpeed             100000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;
uint8_t I2C1_Buffer_Tx[BufferSize] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t I2C2_Buffer_Rx[BufferSize];
uint8_t Tx_Idx = 0, Rx_Idx = 0;
volatile TestStatus TransferStatus;

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
int main(void)
{

	/* Enable I2C1 and I2C2 ----------------------------------------------------*/
	I2C_Cmd(I2C1, ENABLE);
	I2C_Cmd(I2C2, ENABLE);

	/* I2C1 configuration ------------------------------------------------------*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
	I2C_Init(I2C1, &I2C_InitStructure);
	/* I2C2 configuration ------------------------------------------------------*/
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
	I2C_Init(I2C2, &I2C_InitStructure);


	while (1)
	{
		I2C_GenerateSTART(I2C1, ENABLE);
	}
}
