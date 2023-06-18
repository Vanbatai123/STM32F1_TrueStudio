/*
 * Wire.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#include <include.h>
#include "Wire.h"



#define BUFFER_LENGTH 32

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



TwoWire::TwoWire()
{
	// TODO Auto-generated constructor stub

}

TwoWire::~TwoWire()
{
	// TODO Auto-generated destructor stub
}

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

void TwoWire::begin(int address)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;

	// enable GPIO and I2C1 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	// set pin I2C at AF mode
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_I2C1);

	//GPIo config
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//I2C config
	I2C_Cmd(I2C1, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = address;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 30000;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C1, &I2C_InitStructure);
}

void TwoWire::end(void)
{
	//twi_disable();
/*
	// disable twi module, acks, and twi interrupt
	TWCR &= ~((1 << TWEN) | (1 << TWIE) | (1 << TWEA));

	// deactivate internal pullups for twi.
	digitalWrite(SDA, 0);
	digitalWrite(SCL, 0);*/
}

void TwoWire::setClock(uint32_t clock)
{
	//TWBR = ((F_CPU / clock) - 16) / 2;
}

void TwoWire::beginTransmission(int address)
{/*
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
	*/

    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    // Generate start condition
    I2C_GenerateSTART(I2C1, ENABLE);
    // Wait for I2C EV5.
    // It means that the start condition has been correctly released
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));


    // Send slave address
    I2C_Send7bitAddress(I2C1, address<<1, I2C_Direction_Transmitter);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 0;

    // Generate I2C stop condition
    I2C_GenerateSTOP(I2C1, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF))
    {

    }

	return 0;
}

uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
	/*uint8_t i;
	rxBufferLength = quantity;
	rxBufferIndex = 0;

	TWCR=TWI_START; // Start--------------------------------------------------------------------
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x08));	 //cho TWINT bit=1 va goi START thanh cong

	TWDR=(address<<1)+TWI_R; 							 //goi dia chi SLA +READ
	TWCR=TWI_Clear_TWINT; 									 //bat dau, xoa TWINT
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x40));	 //cho TWINT bit=1	va goi SLA thanh cong

	//nhan len-1 bytes dau tien---------------------
	for (i=0; i <quantity-1; i++)
	{
		TWCR=TWI_Read_ACK; 									 //xoa TWINT,se goi ACK sau khi nhan moi byte
		while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x50));//cho TWINT bit=1 hoac nhan duoc ACK
		rxBuffer[i]=TWDR;										  //doc du lieu vao mang Data
	}
	//nhan byte cuoi
	TWCR=TWI_Clear_TWINT; 									 //xoa TWINT de nhan byte cuoi, sau do set NOT ACK
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x58));   //cho TWIN=1 hoac trang thai not ack
	rxBuffer[quantity-1]=TWDR;

	TWCR=TWI_STOP;	*/										 //STOP condition

	// Wait until I2Cx is not busy anymore
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	// Generate start condition
	I2C_GenerateSTART(I2C1, ENABLE);
	// Wait for I2C EV5.
	// It means that the start condition has been correctly released
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));


	// Send slave address
	I2C_Send7bitAddress(I2C1, address<<1, I2C_Direction_Receiver);

	// Wait for I2C EV6
	// It means that a slave acknowledges his address
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    int dataReturn = I2C_ReceiveData(I2C1);


    // Generate I2C stop condition
    I2C_GenerateSTOP(I2C1, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF))
    {

    }

	return dataReturn;


}

int TwoWire::read(void)
{
	/*int value = -1;

	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;*/
	return 1;
}

size_t TwoWire::write(uint8_t val)
{
	/*//send DATA byte---------------------
	TWDR = val; //repare Data on TWDR
	TWCR = TWI_Clear_TWINT; //start send address by cleaning TWINT
	while((TWCR & 0x80)==0x00);		 //check and wait for TWINT bit=1
	if((TWSR & 0xF8) !=_DATA_ACK)
	{
		return (TWSR&0xF8);
	} //data send error, escape anyway
*/

    // Send data byte
    I2C_SendData(I2C1, val);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and
    // output on the bus)
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {

    }

	return 0;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	/*if(transmitting)
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
	}*/
	return quantity;
}
TwoWire Wire = TwoWire();
