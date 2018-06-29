#include "Utility.h"
#include "Constants.h"
#include "DebugConsole.h"
#include "StateTransitions.h"
#include "StateUpdates.h"


/*
Variables
*/
int goSafemodeInterval = 2000;  // ms
int heatbeatInterval = 500;  // ms
int statemachineInterval = 50;  // ms
int accelerationTime = 1000;  // ms


/*
Function declarations
*/
void state_machine_run(uint8_t sensors);
uint8_t calc_next_state();

long prevMillis_check_heartbeat;
long prevMillis_statemachine_update;


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
	roverControlModel.ArmServo0 = { 0.0, 150, 500, 20 };  // steering servo
	roverControlModel.ArmServo1 = { 0.0, 150, 525, 20 };  // steering servo
	roverControlModel.BucketServo = { 0.0, 150, 525, 20 };  // steering servo

	roverControlModel.DriveMotor0.targetSpeed = 0;  // drive motor
	roverControlModel.DriveMotor0.direction = 1;
	roverControlModel.DriveMotor0.accelerationIncrement = floor(255 / (accelerationTime / statemachineInterval));
	roverControlModel.DriveMotor0.pin_ina = PIN_MOTOR0_IN1;
	roverControlModel.DriveMotor0.pin_inb = PIN_MOTOR0_IN2;
	roverControlModel.DriveMotor0.pin_enable = PIN_MOTOR0_ENA;
	
	roverControlModel.DriveMotor1.targetSpeed = 0;  // drive motor
	roverControlModel.DriveMotor1.direction = 1;
	roverControlModel.DriveMotor1.accelerationIncrement = floor(255 / (accelerationTime / statemachineInterval));
	roverControlModel.DriveMotor1.pin_ina = PIN_MOTOR1_IN3;
	roverControlModel.DriveMotor1.pin_inb = PIN_MOTOR1_IN4;
	roverControlModel.DriveMotor1.pin_enable = PIN_MOTOR1_ENB;

	roverControlModel.DriveMotor2.targetSpeed = 0;  // drive motor
	roverControlModel.DriveMotor2.direction = 1;
	roverControlModel.DriveMotor2.accelerationIncrement = floor(255 / (accelerationTime / statemachineInterval));
	roverControlModel.DriveMotor2.pin_ina = PIN_MOTOR2_IN1;
	roverControlModel.DriveMotor2.pin_inb = PIN_MOTOR2_IN2;
	roverControlModel.DriveMotor2.pin_enable = PIN_MOTOR2_ENA;

	roverControlModel.DriveMotor3.targetSpeed = 0;  // drive motor
	roverControlModel.DriveMotor3.direction = 1;
	roverControlModel.DriveMotor3.accelerationIncrement = floor(255 / (accelerationTime / statemachineInterval));
	roverControlModel.DriveMotor3.pin_ina = PIN_MOTOR3_IN3;
	roverControlModel.DriveMotor3.pin_inb = PIN_MOTOR3_IN4;
	roverControlModel.DriveMotor3.pin_enable = PIN_MOTOR3_ENB;

	roverControlModel.MechanismMotor.targetSpeed = 0;  // drive motor
	roverControlModel.MechanismMotor.direction = 1;
	roverControlModel.MechanismMotor.accelerationIncrement = floor(255 / (accelerationTime / statemachineInterval));
	roverControlModel.MechanismMotor.pin_ina = PIN_MOTOR5_IN1;
	roverControlModel.MechanismMotor.pin_inb = PIN_MOTOR5_IN2;
	roverControlModel.MechanismMotor.pin_enable = PIN_MOTOR5_ENA;
	

	
	// ==================================
	// initialise control systems
	// ==================================
	Serial.println("Rover control systems...");
	SetupSPI();
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
	if (roverControlModel.time_since_heartbeat > goSafemodeInterval) {
		//Serial.print("Go into SAFE MODE");
		roverControlModel.state = SAFE;
	}

	/*
	Send telemetry data to Raspberry Pi
	*/
	if ((millis() - prevMillis_check_heartbeat) > heatbeatInterval) {
		// + add send telemetry function
		//Serial.println("Send telemetry");
		prevMillis_check_heartbeat = millis();
	}

	/*
	Update statemachine
	*/
	if ((millis() - prevMillis_statemachine_update) > statemachineInterval) {
		UpdateDebugConsole();
		UpdateSPI();
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
State machine functions
*/
uint8_t calc_next_state()
{
	// + code for reading the data from the comms and determining the next state
	return roverControlModel.state; // for now, set to calibration state
}


