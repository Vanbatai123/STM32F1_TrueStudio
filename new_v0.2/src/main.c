#include <stm32f4xx.h>


/* Private macro */

/* Private variables */
uint8_t fff[7];
uint8_t q;
/* Private function prototypes */
void _delay(__IO uint32_t ms);
/* Private functions */
void _delay(__IO uint32_t ms)
{
	while(ms--)
	{
		int x = 16000;
		while(x--);
	}
}

/*
 * include.c
 *
 *  Created on: Aug 10, 2018
 *      Author: ASUS PC
 */


typedef enum
{
	Error = 0, Success = !Error
} Status;

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte, uint8_t SlaveAddress)
{
	__IO uint32_t Timeout = 0;
	//    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors
	if (!nbyte)
		return Success;
	// Wait for idle I2C interface
	Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	// Enable Acknowledgement, clear POS flag
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

	// Intiate Start Sequence (wait for EV5
	I2C_GenerateSTART(I2Cx, ENABLE);
	Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	// Send Address
	I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

	// EV6
	Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));
	if (nbyte == 1)
	{
		// Clear Ack bit
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		// EV6_1 -- must be atomic -- Clear ADDR, generate STOP
		__disable_irq();
		(void) I2Cx->SR2;
		I2C_GenerateSTOP(I2Cx, ENABLE);
		__enable_irq();

		// Receive data   EV7
		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
		*buf++ = I2C_ReceiveData(I2Cx);

	}
	else if (nbyte == 2)
	{
		// Set POS flag
		I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);

		// EV6_1 -- must be atomic and in this order
		__disable_irq();
		(void) I2Cx->SR2;                           // Clear ADDR flag
		I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
		__enable_irq();

		// EV7_3  -- Wait for BTF, program stop, read data twice
		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

		__disable_irq();
		I2C_GenerateSTOP(I2Cx, ENABLE);
		*buf++ = I2Cx->DR;
		__enable_irq();
		*buf++ = I2Cx->DR;
	}
	else
	{
		(void) I2Cx->SR2;                           // Clear ADDR flag
		while (nbyte-- != 3)
		{
			// EV7 -- cannot guarantee 1 transfer completion time, wait for BTF
			//        instead of RXNE

			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
			*buf++ = I2C_ReceiveData(I2Cx);
		}

		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

		// EV7_2 -- Figure 1 has an error, doesn't read N-2 !

		I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit
		__disable_irq();
		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
		I2C_GenerateSTOP(I2Cx, ENABLE);                  // program stop
		__enable_irq();
		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

		// wait for byte N
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
		*buf++ = I2C_ReceiveData(I2Cx);
		nbyte = 0;
	}
	// Wait for stop
	Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
	return Success;
	errReturn:
	// Any cleanup here
	return Error;
}

/*
 * Read buffer of bytes -- AN2824 Figure 3
 */
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbyte, uint8_t SlaveAddress)
{
	__IO uint32_t Timeout = 0;
	/* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
	//    I2Cx->CR2 |= I2C_IT_ERR;
	if (nbyte)
	{
		Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

		// Intiate Start Sequence
		I2C_GenerateSTART(I2Cx, ENABLE);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

		// Send Address  EV5
		I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		// EV6
		// Write first byte EV8_1
		I2C_SendData(I2Cx, *buf++);
		while (--nbyte)
		{
			// wait on BTF
			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
			I2C_SendData(I2Cx, *buf++);
		}
		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
		I2C_GenerateSTOP(I2Cx, ENABLE);
		Timed(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
	}
	return Success;
	errReturn: return Error;
}

void I2C_Config(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	    I2C_InitTypeDef  I2C_InitStructure;

	    /*enable I2C*/
	    I2C_Cmd(I2C1,ENABLE);

	    /* I2C1 clock enable */
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	    /* I2C1 SDA and SCL configuration */
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_Init(GPIOB, &GPIO_InitStructure);
	    /*SCL is pin06 and SDA is pin 07 for I2C1*/

	    /* I2C1 configuration */
	    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	    I2C_InitStructure.I2C_ClockSpeed = 100000 ;
	    I2C_Init(I2C1, &I2C_InitStructure);
}




/**
**===========================================================================
**  Abstract: main program
**===========================================================================
*/
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//I2C_DeInit(I2C1);


	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 30000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x01;

	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);

	while (1)
	{
		I2C_Read(I2C1,fff,7,0xD0);
		q=fff[0];
		_delay(500);
	}
}
