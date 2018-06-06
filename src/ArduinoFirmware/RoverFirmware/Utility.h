#pragma once
#include "Constants.h"

/*
Conversions
*/

// Convert angle to servo pulse width
uint8_t degToPulse(uint8_t degrees, int min = 150, int max = 500) {
	return map(degrees, 0, 180, min, max);
}