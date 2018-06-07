#pragma once
#include "Constants.h"

/*
Conversions
*/

// Convert angle to servo pulse width
uint8_t degToPulse(uint8_t degrees, int min = 150, int max = 500)
{
	return map(degrees, 0, 180, min, max);
}



/*
Validation functions
*/
bool validatePropValueInt(char* prop)
{
	int len = strlen(prop);
	int i;
	for (i = 0; i < len; i++)
	{
		if (isDigit(prop[i]) == false) {
			return false;
		}
	}
	return true;
}