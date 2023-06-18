/*
 * Wire.cpp
 *
 *  Created on: Aug 9, 2018
 *      Author: ASUS PC
 */

#include <include.h>
#include "Wire.h"

#define BUFFER_LENGTH 32

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
//uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
//void (*TwoWire::user_onRequest)(void);
//void (*TwoWire::user_onReceive)(int);
/*
 * Contruction function of TwoWire class
 */
TwoWire::TwoWire()
{
	// TODO Auto-generated constructor stub
}

/*
 * Destruction function of TwoWire class
 */
TwoWire::~TwoWire()
{
	// TODO Auto-generated destructor stub
}


/*
uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);
*/



void TwoWire::begin(void)
{
	//pinMode(SDA, OUTPUT);
	//pinMode(SCL, OUTPUT);

//	TWSR = 0x00; //Prescaler=1
//	TWBR = _100K;
	//TWAR = (Device_Addr << 1) + GENERAL_CALL;

//	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //ready for Slave mode: Ack is set, TWI is ENABLE and interrupt is ENABLE

}
/*
static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

void twi_attachSlaveTxEvent( void (*function)(void) )
{
	twi_onSlaveTransmit = function;
}

void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
	twi_onSlaveReceive = function;
}

void TwoWire::begin(uint8_t address)
{

	//twi_attachSlaveTxEvent(onRequestService);
	//twi_attachSlaveRxEvent(onReceiveService);

	pinMode(SDA, OUTPUT);
	pinMode(SCL, OUTPUT);

	TWSR = 0x00; //Prescaler=1
	TWBR = _100K;
	TWAR = (address << 1);
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //ready for Slave mode: Ack is set, TWI is ENABLE and interrupt is ENABLE
}

void TwoWire::begin(int address)
{
	begin((uint8_t)address);
}

void TwoWire::end(void)
{
	//twi_disable();

	// disable twi module, acks, and twi interrupt
	TWCR &= ~((1 << TWEN) | (1 << TWIE) | (1 << TWEA));

	// deactivate internal pullups for twi.
	digitalWrite(SDA, 0);
	digitalWrite(SCL, 0);
}

void TwoWire::setClock(uint32_t clock)
{
	TWBR = ((F_CPU / clock) - 16) / 2;
}

void TwoWire::beginTransmission(uint8_t address)
{
	txAddress = address;
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 1;

	//Send START Condition
	TWCR=TWI_START;
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR & 0xF8) !=_START_Sent)
	{
		return;
	}; //START send error, escape anyway

	//Send Device Address---------------------
	TWDR=(txAddress<<1)+TWI_W; //repare address on TWDR,+W (send procedure)
	TWCR=TWI_Clear_TWINT; //start send address by cleaning TWINT
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR&0xF8) !=_SLA_W_ACK)
	{
		return;
	}; //addresssend error, escape anyway

}
*/
void TwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}
/*
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
//	txBufferIndex = 0;
//	txBufferLength = 0;
//	transmitting = 0;

//	TWCR = TWI_STOP;


	return 0;
}

uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}
*/
uint8_t TwoWire::requestFrom(I2C_TypeDef *I2Cx, uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
	rxBufferLength = quantity;
	rxBufferIndex = 0;
	__IO uint32_t Timeout = 0;
	// I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors
	if (!quantity)
		return 1;

	// Wait for idle I2C interface
	Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	// Enable Acknowledgement, clear POS flag
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

	// Intiate Start Sequence (wait for EV5)
	I2C_GenerateSTART(I2Cx, ENABLE);
	Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	// Send Address
	I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Receiver);
	// EV6
	Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));

	if (quantity == 1)
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
		*(rxBuffer + rxBufferIndex++) = I2C_ReceiveData(I2Cx);
	}
	else if (quantity == 2)
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
		*(rxBuffer + rxBufferIndex++) = I2Cx->DR;
		__enable_irq();
		*(rxBuffer + rxBufferIndex++) = I2Cx->DR;
	}
	else
	{
		(void) I2Cx->SR2;                           // Clear ADDR flag
		while (quantity-- != 3)
		{
			// EV7 -- cannot guarantee 1 transfer completion time, wait for BTF
			//        instead of RXNE
			Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
			*(rxBuffer + rxBufferIndex++) = I2C_ReceiveData(I2Cx);
		}
		Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

		// EV7_2 -- Figure 1 has an error, doesn't read N-2 !
		I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit
		__disable_irq();
		*(rxBuffer + rxBufferIndex++) = I2C_ReceiveData(I2Cx);             // receive byte N-2
		I2C_GenerateSTOP(I2Cx, ENABLE);                  // program stop
		__enable_irq();
		*(rxBuffer + rxBufferIndex++) = I2C_ReceiveData(I2Cx);             // receive byte N-1

		// wait for byte N
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
		*(rxBuffer + rxBufferIndex++) = I2C_ReceiveData(I2Cx);
		quantity = 0;
	}

	rxBufferIndex = 0;
	// Wait for stop
	Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
	return 1;
	errReturn:
	// Any cleanup here
	return 0;
}

uint8_t TwoWire::requestFrom(I2C_TypeDef *I2Cx, uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	return requestFrom(I2Cx, (uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(I2C_TypeDef *I2Cx, uint8_t address, uint8_t quantity)
{
	return requestFrom(I2Cx, (uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(I2C_TypeDef *I2Cx, int address, int quantity)
{
	return requestFrom(I2Cx, (uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(I2C_TypeDef *I2Cx, int address, int quantity, int sendStop)
{
	return requestFrom(I2Cx, (uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

int TwoWire::read(void)
{
	int value = -1;

	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}
/*
size_t TwoWire::write(uint8_t val)
{
	//send DATA byte---------------------
	TWDR = val; //repare Data on TWDR
	TWCR = TWI_Clear_TWINT; //start send address by cleaning TWINT
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR & 0xF8) !=_DATA_ACK)
	{
		return (TWSR&0xF8);
	} //data send error, escape anyway

	return 0;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	if(transmitting)
	{
		// in master transmitter mode
		for(size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	else
	{
		beginTransmission(txAddress);
		for(size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	return quantity;
}

*/
