/*
 *
 * Project convert STM32 to Arduino
 *
 *  Created on: Aug 8, 2018
 *      Author: Van_BasTai
 */


#include <include.h>
#include <Wire/Wire.h>
#include <GPIO/GPIO.h>




/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define I2C1_DR_Address        0x40005410
#define I2C2_DR_Address        0x40005810
#define I2C1_SLAVE_ADDRESS7    0x68
#define I2C2_SLAVE_ADDRESS7    0x68
#define BufferSize             7
#define ClockSpeed             100000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;
uint8_t I2C1_Buffer_Tx[BufferSize] = {1, 2, 3, 4, 5, 6, 7};
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


	Wire.begin();

	while (1)
	{
		//begin transmission
		I2C_GenerateSTART(I2C1, ENABLE);
		I2C_Send7bitAddress(I2C1, I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);

		// write data
		I2C_SendData(I2C1, 0x00);

		// end transmission
		I2C_GenerateSTOP(I2C1, ENABLE);

		// request from

		Rx_Idx = 0;
		I2C_GenerateSTART(I2C1, ENABLE);
		for(uint8_t i = 0; i < BufferSize; ++i)
		{
			I2C2_Buffer_Rx[i] = I2C_ReceiveData(I2C1);
		}
		I2C_GenerateSTOP(I2C1, ENABLE);


		uint8_t timeDate[7];
		// read data
		for (int i = 0; i < 7; ++i)
		{
			// get each successive byte on each call
			if (Rx_Idx < BufferSize)
			{
				timeDate[i] = I2C2_Buffer_Rx[Rx_Idx];
				++Rx_Idx;
			}
		}
		pinMode(GD,P12,1);
		a();
//		delay(1000);
	}
}
