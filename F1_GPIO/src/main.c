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
int main(void)
{

	pinMode(GC,P13,1);
	while(1)
	{
		digitalToggle(GC,P13);
		delay(1000);
	}
}
