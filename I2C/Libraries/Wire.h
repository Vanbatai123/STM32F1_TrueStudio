/*
 * Wire.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ASUS PC
 */

#ifndef WIRE_H_
#define WIRE_H_


#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}



class TwoWire
{
	public:
	TwoWire();
	virtual ~TwoWire();

	void begin();
	/*void begin(int addr);
	void end();
	void setClock(uint32_t);
*/
	void beginTransmission(int);
	uint8_t endTransmission(void);


	uint8_t requestFrom(I2C_TypeDef*, int, int);
/*	size_t write(uint8_t);
	size_t write(const uint8_t *, size_t);

	int available(void);
*/
	int read(void);
/*	int peek(void);
	void flush(void);
	void onReceive( void (*)(int) );
	void onRequest( void (*)(void) );

	private:
*/
	static uint8_t rxBuffer[];
	static uint8_t rxBufferIndex;
	static uint8_t rxBufferLength;

	static uint8_t txAddress;
	static uint8_t *txBuffer;
	static uint8_t txBufferIndex;
	static uint8_t txBufferLength;
	static uint8_t transmitting;

	I2C_TypeDef _I2Cx;
/*
	void begin(uint8_t);
	void beginTransmission(uint8_t);
	uint8_t endTransmission(uint8_t);
*/	uint8_t requestFrom(I2C_TypeDef*, uint8_t, uint8_t, uint8_t);
	uint8_t requestFrom(I2C_TypeDef*, uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
	uint8_t requestFrom(I2C_TypeDef*, int, int, int);
	uint8_t requestFrom(I2C_TypeDef*, uint8_t, uint8_t);

/*

	static void (*user_onRequest)(void);
	static void (*user_onReceive)(int);
	static void onRequestService(void);
	static void onReceiveService(uint8_t*, int);*/
};

#endif /* WIRE_H_ */
