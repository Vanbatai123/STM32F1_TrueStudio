/*
 *
 * Project convert STM32 to Arduino
 *
 *	Done: SPI1, USART1, I2C1, ADC1, ADC_DMA, EXTI, Stepper
 *	Undone: DMA, Interrupt, ADC, Timer...
 *
 * 	example: SD
 *  Created on: Aug 8, 2018
 *      Author: Van_BasTai
 *
 *      file include.h thay the cho file Arduino.h
 */

/* include */

#include <core/include.h>
#include <Serial/Serial.h>
#include <Wire/Wire.h>
#include <AccelStepper/AccelStepper.h>
#include <SPI/SPIClass.h>
#include <SD/SD.h>
//#include <SPI/SPIClass.h>
//#include <RFID/MFRC522.h>
//#include <Liquid_LCD/LiquidCrystal_SPi.h>


/* Private define */

/* Private typedef */

/* Private function prototypes */

/* Private functions */

/* Private variable */
uint32_t ttt;
uint8_t bufData[20], cnt;
union MyUnion
{
	byte b[4];
	float f;
}posX, posY, posF;
/************************************************************/
/*                  MAIN FUNCTION							*/
/************************************************************/
// Define some steppers and the pins the will use
AccelStepper stepper(1,A5,A4);// pin 5 step, pin 4 dir

int main(void)
{
	SysTick_Init();
	Serial.begin(115200);
	Serial.println("READY!!!!!!");

	pinMode(6,OUTPUT);
	pinMode(5,OUTPUT); // Step pin
	pinMode(4,OUTPUT); // Dir - pin
	digitalWrite(6,LOW); // Set Enable low
	while (1)
	{
		digitalWrite(6,LOW);
		digitalWrite(4,HIGH);
		Serial.println("Cho chay 200 steps (1 vong)");

//		Serial.println("Pause");
		delay(300);
	}
}
