#pragma once

/* 
Pins - This is where we define all the pins that will be used
*/
#define DEBUG_LED_PIN 49
#define PIN_MOTOR1 5
#define PIN_MOTOR2 6
#define PIN_MOTOR3 7
#define PIN_MOTOR4 8



/*
State machine related stuff
*/
enum State_enum { SAFE, IDLE, LOCOMOTION, HARVEST, STORE, CALIBRATE_SERVOS };
//enum Sensors_enum { NONE, SENSOR_RIGHT, SENSOR_LEFT, BOTH };



/*
Communication related stuff
*/
struct Messages
{
	enum Types_enum { HEARTBEAT, TELEMETRY, EMERGENCY_STOP, SET_MOTOR_SPEED, SET_MOTOR_STEERING };
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

