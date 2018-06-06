#ifdef _WIN32 // if on windows, include this
#include <iostream> 
#endif

#include "Constants.h"
#include "MotorControl.h"
#include "ServoControl.h"

/*
Function declarations
*/
void state_machine_run(uint8_t sensors);
uint8_t calc_next_state();

void transition_safe();
void transition_idle();
void transition_locomotion();
void transition_harvest();
void transition_store();
void update_safe();
void update_idle();
void update_locomotion();
void update_harvest();
void update_store();

uint8_t state;
long time_since_heartbeat;
long prevMillis_check_heartbeat;

// DEBUG
unsigned long debug_time;        //time from millis()
unsigned long prev_debug_time;    //last time the LED changed state
boolean debugLedState;        //current LED state

void setup()
{
#ifdef _WIN32 // if on windows, run this
	std::cout << "[1 FIRMWARE] Setup" << std::endl;
#endif
	pinMode(53, OUTPUT);
	state = SAFE;
}



void loop()
{
#ifdef _WIN32 // if on windows, run this
	std::cout << "[1 FIRMWARE] Loop: " << iterations << std::endl;
#endif
	/*
	Send telemetry
	*/
	if ((millis() - prevMillis_check_heartbeat) > 1000) {
		// add send telemetry function
		prevMillis_check_heartbeat = millis();
	}


	/*
	Update statemachine
	*/
	state_machine_run(calc_next_state());

	delay(10);
}


/*
State machine
*/
void state_machine_run(uint8_t next_state)
{
	// set mode to SAFE since we haven't heard from the heartbeat in more than 2 seconds
	if (time_since_heartbeat > 2000) {
		state = SAFE;
	}

	switch (state)
	{
	case SAFE:
		if (next_state == IDLE) {
			transition_idle();
			state = IDLE;
		}
		update_safe();
		break;

	case IDLE:
		if (next_state == LOCOMOTION) {
			transition_locomotion();
			state = LOCOMOTION;
		}
		else if (next_state == HARVEST) {
			transition_harvest();
			state = HARVEST;
		}
		else if (next_state == SAFE) {
			transition_safe();
			state = SAFE;
		}
		else if (next_state == STORE) {
			transition_store();
			state = STORE;
		}
		update_idle();
		break;

	case LOCOMOTION:
		if (next_state == SAFE) {
			transition_safe();
			state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			state = IDLE;
		}
		update_locomotion();
		break;

	case HARVEST:
		if (next_state == SAFE) {
			transition_safe();
			state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			state = IDLE;
		}
		else if (next_state == STORE) {
			transition_store();
			state = STORE;
		}
		update_harvest();
		break;

	case STORE:
		if (next_state == SAFE) {
			transition_safe();
			state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			state = IDLE;
		}
		update_store();
		break;
	}
}

/*
Utility functions
*/
void DEBUG_LED_flash(int ontime, float offtime = 200)
{
	if (debug_time - prev_debug_time > (debugLedState ? ontime : offtime)) {
		digitalWrite(53, debugLedState = !debugLedState);
		prev_debug_time = debug_time;
	}
}


/*
State machine functions
*/
uint8_t calc_next_state()
{
	//code for reading the data from the comms and determining the next state
	return 1;
}



/*
State transition functions - These are the state transition functions that are called before the state is updated
*/
void transition_safe()
{
	// stop everything without reseting positions or anything. just stop everything except communication.

}

void transition_idle()
{
	// set motor speed to 0
	// set motor steering to 0
	// set harvesting mechanism speed to 0
	// stow harvesting mechanism

}

void transition_locomotion()
{
	// stow harvesting mechanism

}

void transition_harvest()
{
	// update motor speeds
	// update steering angles
	// update harvesting mechanism

}

void transition_store()
{
	// set motor speed and steering to 0
	// move mechanism to store position

}


/*
State functions - These are the update functions for each state we've defined.
*/
void update_safe()
{
	// make sure nothing updates
	// only allow communication and telemetry
	DEBUG_LED_flash(100);
}

void update_idle()
{
	// make sure motors, mechanism, and steering at set to 0
	// update harvesting stowing until it is fully stowed
	DEBUG_LED_flash(200);
}

void update_locomotion()
{
	// update motors and steering to match the messages received
	DEBUG_LED_flash(400);
}

void update_harvest()
{
	// update speed, steering, and harvesting mechanism
	DEBUG_LED_flash(800);
}

void update_store()
{
	// make sure motor speed and steering is zero
	// set mechanism speed to release the material gathered
	DEBUG_LED_flash(1600);
}




