#include <include.h>
#include <Wire/Wire.h>

/* Private macro */
/* Private variables */

/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
uint8_t dataa = 0;
int main(void)
{
	Wire.begin(0x12);
	while(1)
	{
		Wire.beginTransmission(0x68);
		Wire.write(0x00);
		Wire.endTransmission();
		dataa = Wire.requestFrom(0x68, 1);
		digitalToggle(GC,P13);
		delay(1000);
	}
}
