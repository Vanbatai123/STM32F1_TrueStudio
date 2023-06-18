/*
 * Wire.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#include <include.h>
#include <Wire/Wire.h>
#include <Wire/twi/twi.h>
#include <Serial/Serial.h>


static volatile voidFuncPtr onRequestFunc = NULL;
static volatile voidFuncPtr onReceiveFunc = NULL;

#define BUFFER_LENGTH 32

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t status = 0;
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

void TwoWire::begin()
{
	begin(0x00);
}
void TwoWire::begin(uint8_t address)
{
	rxBufferIndex = 0;
	rxBufferLength = 0;

	txBufferIndex = 0;
	txBufferLength = 0;

	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the I2C1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //Configure I2C error interrupt to have the higher priority.
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_Init(&NVIC_InitStructure);

	// enable GPIO and I2C1 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(I2C1_RCC_GPIO | I2C1_RCC_AFIO, ENABLE);

	//GPIo config
	GPIO_InitStructure.GPIO_Pin = I2C1_PIN_SCL | I2C1_PIN_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C1_GPIO, &GPIO_InitStructure);

	//I2C config
	Twi_Cmd(I2C1, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = address<<1;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 30000;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	Twi_Init(I2C1, &I2C_InitStructure);


}

void TwoWire::end(void)
{
	Twi_DeInit(I2C1);
}

void TwoWire::setClock(uint32_t clock)
{
	//TWBR = ((F_CPU / clock) - 16) / 2;
}

void TwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}

void TwoWire::beginTransmission(uint8_t address)
{
	// indicate that we are transmitting
	transmitting = 1;
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;

    // Wait until I2Cx is not busy anymore
    while (Twi_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    // Generate start condition
    Twi_GenerateSTART(I2C1, ENABLE);
    // Wait for I2C EV5.
    // It means that the start condition has been correctly released
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send slave address
    Twi_Send7bitAddress(I2C1, address<<1, I2C_Direction_Transmitter);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
	txBufferIndex = 0;
	txBufferLength = 0;
	transmitting = 0;

    // Generate I2C stop condition
    Twi_GenerateSTOP(I2C1, ENABLE);
    // Wait until I2C stop condition is finished
    while (Twi_GetFlagStatus(I2C1, I2C_FLAG_STOPF))
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
	requestFrom((uint8_t)address, (uint8_t)quantity);
	return 1;
}
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
	uint8_t i;
	rxBufferLength = quantity;
	rxBufferIndex = 0;

	// Wait until I2Cx is not busy anymore
	while (Twi_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	// Generate start condition
	Twi_GenerateSTART(I2C1, ENABLE);
	// Wait for I2C EV5.
	// It means that the start condition has been correctly released
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave address
	Twi_Send7bitAddress(I2C1, address<<1, I2C_Direction_Receiver);

	// Wait for I2C EV6
	// It means that a slave acknowledges his address
	while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	// receive quantity-1 byte

	for (i = 0 ; i <quantity-1; i++)
	{
	    // Enable ACK of received data
	    Twi_AcknowledgeConfig(I2C1, ENABLE);
	    // Wait for I2C EV7
	    // It means that the data has been received in I2C data register
	    while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	    // Read and return data byte from I2C data register
	    rxBuffer[i] = Twi_ReceiveData(I2C1);
	}

    // Disable ACK of received data
    Twi_AcknowledgeConfig(I2C1, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    rxBuffer[quantity - 1] = Twi_ReceiveData(I2C1);


    // Generate I2C stop condition
    Twi_GenerateSTOP(I2C1, ENABLE);
    // Wait until I2C stop condition is finished
    while (Twi_GetFlagStatus(I2C1, I2C_FLAG_STOPF)){}
	return 1;
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

size_t TwoWire::write(uint8_t val)
{
    // Send data byte
    Twi_SendData(I2C1, val);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and
    // output on the bus)
    while (!Twi_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){}

	return 0;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	switch (status)
	{
		case I2C_STT_SLAVE_TRANSMIT:
			for(size_t i = 0; i < quantity; ++i)
			{
				Twi_SendData(I2C1, data[i]);
				if(i != quantity - 1)
					while (!Twi_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_TRANSMITTED)){}
			}

			break;

		case I2C_STT_MASTER_TRANSMIT:
			if(transmitting)
			{
				// in master transmitter mode
				for(size_t i = 0; i < quantity; ++i) write(data[i]);
			}
			else
			{
				beginTransmission(txAddress);
				for(size_t i = 0; i < quantity; ++i) write(data[i]);
			}
			break;

		default:
			break;
	}


	return quantity;
}

int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(void) )
{
	onReceiveFunc = function;
	// enable receive interrupt
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE); //Part of the STM32 I2C driver
    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE); //Part of the STM32 I2C driver
    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE); //Part of the STM32 I2C driver
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
	onRequestFunc = function;
	// enable receive interrupt
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE); //Part of the STM32 I2C driver
    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE); //Part of the STM32 I2C driver
    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE); //Part of the STM32 I2C driver
}

void I2C1_EV_IRQHandler(void)
{
	//uint32_t event = I2C_GetLastEvent(I2C1);
	//Serial.println(event,HEX);
	switch ( I2C_GetLastEvent(I2C1))
	{
		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
			status = I2C_STT_SLAVE_RECEIVE;
			break;

		case I2C_EVENT_SLAVE_BYTE_RECEIVED|I2C_SR1_BTF:
			if(onReceiveFunc != NULL)
				onReceiveFunc();
			else I2C_ReceiveData(I2C1);
			break;

		case 0x0040:
			if(onReceiveFunc != NULL)
				onReceiveFunc();
			else I2C_ReceiveData(I2C1);
			break;

		case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
			//Serial.println("I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED");
			status = I2C_STT_SLAVE_TRANSMIT;
			if(onRequestFunc != NULL)
				onRequestFunc();
			break;

		case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
			//Serial.println("I2C_EVENT_SLAVE_BYTE_TRANSMITTED");
			break;

		case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
			//Serial.println("I2C_EVENT_SLAVE_BYTE_TRANSMITTING");
			break;

		case I2C_EVENT_SLAVE_STOP_DETECTED:
			// Master has STOP sent
			Twi_ClearFlag(I2C1, I2C_SR1_ADDR);
			Twi_ClearFlag(I2C1, I2C_SR1_STOPF);
			break;

		default:
			break;
	}

	//Serial.println((char)I2C_ReceiveData(I2C1));
	// Master has STOP sent
	Twi_ClearFlag(I2C1, I2C_SR1_ADDR);
	Twi_ClearFlag(I2C1, I2C_SR1_STOPF);
}

/*******************************************************************/
void I2C1_ER_IRQHandler(void)
{
	if (I2C_GetITStatus(I2C1, I2C_IT_AF))
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
	}
}
/*******************************************************************/

TwoWire Wire;
