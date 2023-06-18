/*
 * Wire.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ASUS PC
 */

#ifndef WIRE_WIRE_H_
#define WIRE_WIRE_H_

typedef enum
{
	Error = 0, Success = !Error
} Status;

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte, uint8_t addr);
class TwoWire
{
public:
	TwoWire();
	virtual ~TwoWire();

	void I2C_Config(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress);
	Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte, uint8_t addr);
	Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbyte,	uint8_t addr);
};

#endif /* WIRE_WIRE_H_ */
