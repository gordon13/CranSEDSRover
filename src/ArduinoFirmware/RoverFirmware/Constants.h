#pragma once

/* 
Pins - This is where we define all the pins that will be used
*/
#define DEBUG_LED_PIN 49

// The following motor pins are relating to the HW295 motor driver. One controller controls two motors hence the *_1..4 numbering scheme
#define PIN_MOTOR0_IN1 30  
#define PIN_MOTOR0_IN2 31
#define PIN_MOTOR0_ENA 32

#define PIN_MOTOR1_IN3 33
#define PIN_MOTOR1_IN4 34
#define PIN_MOTOR1_ENB 35

// sensors
#define PIN_SENSOR_TEMPERATURE 22
#define PIN_SENSOR_VOLTAGE 23



/*
State machine related stuff
*/
enum State_enum { SAFE, IDLE, LOCOMOTION, HARVEST, STORE, CALIBRATE_SERVOS, TOTAL_STATES };
//enum Sensors_enum { NONE, SENSOR_RIGHT, SENSOR_LEFT, BOTH };



/*
Communication related stuff
*/
#define SERIAL_INPUT_SIZE 30


struct Messages
{
	enum Types_enum { HEARTBEAT, TELEMETRY, EMERGENCY_STOP, SET_MOTOR_SPEED, SET_MOTOR_STEERING, TOTAL_MESSAGE_TYPES};
};






















/*messages = [
	{
		"name": "heartbeat",
			"type" : 0, # also known as type
			"binary_format_string": "h",
			"description" : "heartbeat"
	},
	{
		"name": "telemetry",
		"type" : 1,
		"binary_format_string" : "hh",
		"description" : "telemetry: battery_voltage, battery_temperature, ..."
	},
	{
		"name": "emergency_stop",
		"type" : 2,
		"binary_format_string" : "h",
		"description" : "emergency_stop"
	},
	{
		"name": "set_motor_speed",
		"type" : 3,
		"binary_format_string" : "hf",
		"description" : "set_motor_speed: motor_index, motor_speed"
	},
	{
		"name": "set_motor_steering",
		"type" : 4,
		"binary_format_string" : "hf",
		"description" : "set_motor_steering: motor_index, motor_speed"
	}
]*/

