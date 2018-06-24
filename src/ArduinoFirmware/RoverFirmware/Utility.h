#pragma once
#include "Constants.h"

// DEBUG
unsigned long debug_time;        //time from millis()
unsigned long prev_debug_time;    //last time the LED changed state
boolean debugLedState;        //current LED state


/*
Debug functions
*/
void DEBUG_LED_flash(int ontime, float offtime = 200)
{
	debug_time = millis();
	if (debug_time - prev_debug_time > (debugLedState ? ontime : offtime)) {
		digitalWrite(DEBUG_LED_PIN, debugLedState = !debugLedState);
		prev_debug_time = debug_time;
	}
}


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