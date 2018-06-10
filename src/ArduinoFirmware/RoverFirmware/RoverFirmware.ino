#include "Utility.h"
#include "RoverControlModel.h"
#include "Constants.h"
#include "ServoControl.h"
#include "DebugConsole.h"
#include "StateTransitions.h"

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
	roverControlModel.DriveMotor0.targetSpeed = 0;  // drive motor
	roverControlModel.DriveMotor0.direction = 1;
	roverControlModel.DriveMotor0.accelerationTime = 1;
	roverControlModel.DriveMotor0.pin_ina = PIN_MOTOR0_IN1;
	roverControlModel.DriveMotor0.pin_inb = PIN_MOTOR0_IN2;
	roverControlModel.DriveMotor0.pin_enable = PIN_MOTOR0_ENA;
	roverControlModel.DriveMotor1.targetSpeed = 0;  // drive motor

	roverControlModel.DriveMotor1.direction = 1;
	roverControlModel.DriveMotor1.accelerationTime = 1;
	roverControlModel.DriveMotor1.pin_ina = PIN_MOTOR1_IN3;
	roverControlModel.DriveMotor1.pin_inb = PIN_MOTOR1_IN4;
	roverControlModel.DriveMotor1.pin_enable = PIN_MOTOR1_ENB;
	

	
	// ==================================
	// initialise control systems
	// ==================================
	Serial.println("Rover control systems...");
	BatteryControlSetup();
	MotorControlSetup();
	ServoControlSetup();

	Serial.println("Ready.");
	delay(1000);
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
	roverControlModel.SteeringServo0.angle = 0;
	roverControlModel.SteeringServo1.angle = 0;
	roverControlModel.DriveMotor0.targetSpeed = 0;
	roverControlModel.DriveMotor1.targetSpeed = 0;
	ServoControlUpdate();
	MotorControlUpdate();
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




