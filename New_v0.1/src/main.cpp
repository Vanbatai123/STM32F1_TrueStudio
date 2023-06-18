#include <include/include.h>

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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

	I2C_DeInit(I2C1);

	I2C_Cmd(I2C1, ENABLE);

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 30000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0;

	I2C_Init(I2C1, &I2C_InitStructure);
	while (1)
	{
		I2C_Read(I2C1,fff,7,0xD0);
		q=fff[0];
		_delay(500);
	}
}
