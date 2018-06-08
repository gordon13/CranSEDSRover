#include "Utility.h"
#include "RoverControlModel.h"
#include "Constants.h"
#include "MotorControl.h"
#include "ServoControl.h"
#include "DebugConsole.h"
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

long prevMillis_check_heartbeat;
long prevMillis_statemachine_update;

// DEBUG
unsigned long debug_time;        //time from millis()
unsigned long prev_debug_time;    //last time the LED changed state
boolean debugLedState;        //current LED state

/*
Initialisation
- This is where everything initialises
*/
void setup()
{
	Serial.begin(9600);
	Serial.println("Initialising...");

	prevMillis_check_heartbeat = millis();
	prevMillis_statemachine_update = millis();
	prev_debug_time = millis();

	// debug pins
	pinMode(DEBUG_LED_PIN, OUTPUT);

	// ==================================
	// initialise the rover control model
	// ==================================
	Serial.println("Rover state model...");
	roverControlModel.state = SAFE;  // always default to SAFE
	transition_safe();  // fire safe transition
	roverControlModel.SteeringServo0 = { 0.0, 150, 500, 20 };  // steering servo
	roverControlModel.SteeringServo1 = { 0.0, 150, 525, 20 };  // steering servo
	roverControlModel.DriveMotor0.speed = 0;  // drive motor
	roverControlModel.DriveMotor0.direction = 1; 
	roverControlModel.DriveMotor1.speed = 0;  // drive motor
	roverControlModel.DriveMotor1.direction = 1;
	

	
	// ==================================
	// initialise control systems
	// ==================================
	Serial.println("Rover control systems...");
	MotorControlSetup();
	ServoControlSetup();

	Serial.println("Ready.");
}



/*
Main loop
- This is where everything starts
*/
void loop()
{
	// set mode to SAFE since we haven't heard from the heartbeat in more than 2 seconds
	if (roverControlModel.time_since_heartbeat > 2000) {
		//Serial.print("Go into SAFE MODE");
		roverControlModel.state = SAFE;
	}

	/*
	Send telemetry data to Raspberry Pi
	*/
	if ((millis() - prevMillis_check_heartbeat) > 500) {
		// + add send telemetry function
		//Serial.println("Send telemetry");
		prevMillis_check_heartbeat = millis();
	}

	/*
	Update statemachine
	*/
	String command;
	if ((millis() - prevMillis_statemachine_update) > 50) {
		UpdateDebugConsole();
		state_machine_run(calc_next_state());
		prevMillis_statemachine_update = millis();
	}

}





/*
State machine
*/
void state_machine_run(uint8_t next_state)
{
	switch (roverControlModel.state)
	{
	case SAFE:
		if (next_state == IDLE) {
			transition_idle();
			roverControlModel.state = IDLE;
		}
		if (next_state == CALIBRATE_SERVOS) {
			// transition_****(); // TODO - add calibration transition?
			roverControlModel.state = CALIBRATE_SERVOS;
		}
		update_safe();
		break;

	case IDLE:
		if (next_state == LOCOMOTION) {
			transition_locomotion();
			roverControlModel.state = LOCOMOTION;
		}
		else if (next_state == HARVEST) {
			transition_harvest();
			roverControlModel.state = HARVEST;
		}
		else if (next_state == SAFE) {
			transition_safe();
			roverControlModel.state = SAFE;
		}
		else if (next_state == STORE) {
			transition_store();
			roverControlModel.state = STORE;
		}
		update_idle();
		break;

	case LOCOMOTION:
		if (next_state == SAFE) {
			transition_safe();
			roverControlModel.state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			roverControlModel.state = IDLE;
		}
		update_locomotion();
		break;

	case HARVEST:
		if (next_state == SAFE) {
			transition_safe();
			roverControlModel.state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			roverControlModel.state = IDLE;
		}
		else if (next_state == STORE) {
			transition_store();
			roverControlModel.state = STORE;
		}
		update_harvest();
		break;

	case STORE:
		if (next_state == SAFE) {
			transition_safe();
			roverControlModel.state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			roverControlModel.state = IDLE;
		}
		update_store();
		break;

	case CALIBRATE_SERVOS:
		if (next_state == SAFE) {
			transition_safe();
			roverControlModel.state = SAFE;
		}
		else if (next_state == IDLE) {
			transition_idle();
			roverControlModel.state = IDLE;
		}
		update_calibrate_servos();
		break;
	}
}

/*
Utility functions
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
State machine functions
*/
uint8_t calc_next_state()
{
	// + code for reading the data from the comms and determining the next state
	return roverControlModel.state; // for now, set to calibration state
}



/*
State transition functions - These are the state transition functions that are called before the state is updated
*/
void transition_safe()
{
	// + stop everything without reseting positions or anything. just stop everything except communication.
	Serial.println("Transition -> SAFE");
}

void transition_idle()
{
	// + set motor speed to 0
	// + set motor steering to 0
	// + set harvesting mechanism speed to 0
	// + stow harvesting mechanism
	roverControlModel.DriveMotor0Speed = 0;
	//roverControlModel.SteeringServo0Angle = 0;
	Serial.println("Transition -> IDLE");
}

void transition_locomotion()
{
	// + stow harvesting mechanism
	Serial.println("Transition -> LOCOMOTION");

}

void transition_harvest()
{
	// + update motor speeds
	// + update steering angles
	// + update harvesting mechanism
	Serial.println("Transition -> HARVEST");
}

void transition_store()
{
	// + set motor speed and steering to 0
	// + move mechanism to store position
	Serial.println("Transition -> STORE");
}



/*
State functions - These are the update functions for each state we've defined.
*/
void update_safe()
{
	//Serial.println("Update safe LED");
	// + make sure nothing updates
	// + only allow communication and telemetry
	DEBUG_LED_flash(1500);
	//delay(1000);
}

void update_idle()
{
	//Serial.println("Update idle LED");
	// + make sure motors, mechanism, and steering at set to 0
	// + update harvesting stowing until it is fully stowed
	DEBUG_LED_flash(100);
	ServoControlUpdate();
	//delay(1000);
}

void update_locomotion()
{
	// + update motors and steering to match the messages received
	//DEBUG_LED_flash(400);
	MotorControlUpdate();
}

void update_harvest()
{
	// + update speed, steering, and harvesting mechanism
	//DEBUG_LED_flash(800);
}

void update_store()
{
	// + make sure motor speed and steering is zero
	// + set mechanism speed to release the material gathered
	//DEBUG_LED_flash(1600);
}

void update_calibrate_servos()
{
	ServoControlCalibrateUpdate();
}




