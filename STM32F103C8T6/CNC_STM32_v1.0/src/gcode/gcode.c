/*
* stepper.c
*
* Created: 10/26/2018 9:09:27 PM
*  Author: PC
*/
#include "../include.h"
#include "gcode.h"

struct gValue getAllGcodeValue(char *gCode)
{
	struct gValue gval1;
	
	gval1.G = getGcodeValue(gCode, 'G') / 100;
	gval1.X = getGcodeValue(gCode, 'X');
	gval1.Y = getGcodeValue(gCode, 'Y');
	gval1.Z = getGcodeValue(gCode, 'Z');
	gval1.I = getGcodeValue(gCode, 'I');
	gval1.J = getGcodeValue(gCode, 'J');
	gval1.R = getGcodeValue(gCode, 'R');
	
	return gval1;
}


int16_t getGcodeValue(char *gCode, char gValue)
{
	char *gTemp;
	gTemp = malloc(7);
	uint8_t j = 0, exists = false;
	
	for (uint8_t i = 0; gCode[i]!= '\0'; i++){
		if (exists == true){
			if (gCode[i] != ' '){
				gTemp[j++] = gCode[i];
			}
			else{
				gTemp[j] = '\0';
				break;
			}
		}
		else if (gCode[i] == gValue){
			exists = true;
		}
	}
	
	int16_t reValue = atof(gTemp) * 100;
	free(gTemp);
	return reValue;
}
