/*
 * Wire.h
 *
 *  Created on: Aug 10, 2018
 *      Author: taivb
 */

#ifndef WIRE_WIRE_H_
#define WIRE_WIRE_H_

#ifdef __cplusplus

#define I2C_STT_SLAVE_TRANSMIT	0x20
#define I2C_STT_SLAVE_RECEIVE	0x21
#define I2C_STT_MASTER_TRANSMIT	0x22
#define I2C_STT_SLAVE_TRANSMIT	0x20


class TwoWire
{
public:
	TwoWire();
	virtual ~TwoWire();

	void begin();
	void begin(uint8_t address);
	void end();
	void setClock(uint32_t);

	void beginTransmission(uint8_t address);
	void beginTransmission(int address);
	uint8_t endTransmission(void);

	uint8_t requestFrom(int, int);
	uint8_t requestFrom(uint8_t, uint8_t);
	size_t write(uint8_t);
	size_t write(const uint8_t *, size_t);

	int available(void);
	int read(void);
	int peek(void);
	void flush(void);
	void onReceive( void (*)(void));
	void onRequest( void (*)(void));

private:
	uint8_t endTransmission(uint8_t);

	static uint8_t rxBuffer[];
	static uint8_t rxBufferIndex;
	static uint8_t rxBufferLength;

	static uint8_t txAddress;
	static uint8_t txBuffer[];
	static uint8_t txBufferIndex;
	static uint8_t txBufferLength;

	static uint8_t transmitting;
	static void (*user_onRequest)(void);
	static void (*user_onReceive)(int);
//	static void onRequestService(void);
//	static void onReceiveService(uint8_t*, int);
};


extern TwoWire Wire;

#endif

#endif /* WIRE_WIRE_H_ */
