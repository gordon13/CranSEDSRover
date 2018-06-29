#pragma once

/* 
Pins - This is where we define all the pins that will be used
*/
#define DEBUG_LED_PIN LED_BUILTIN

// The following motor pins are relating to the HW295 motor driver. One controller controls two motors hence the *_1..4 numbering scheme
#define PIN_MOTOR0_IN1 30  
#define PIN_MOTOR0_IN2 31
#define PIN_MOTOR0_ENA 8  // must be PWM

#define PIN_MOTOR1_IN3 33
#define PIN_MOTOR1_IN4 34
#define PIN_MOTOR1_ENB 9  // must be PWM

#define PIN_MOTOR2_IN1 35
#define PIN_MOTOR2_IN2 36
#define PIN_MOTOR2_ENA 10  // must be PWM

#define PIN_MOTOR3_IN3 37
#define PIN_MOTOR3_IN4 38
#define PIN_MOTOR3_ENB 11  // must be PWM

#define PIN_MOTOR5_IN1 39
#define PIN_MOTOR5_IN2 40
#define PIN_MOTOR5_ENA 12  // must be PWM

// sensors
#define PIN_SENSOR_TEMPERATURE 7
#define PIN_SENSOR_VOLTAGE 23

// SPI
#define PIN_SS 53
#define PIN_SCK 52
#define PIN_MOSI 51
#define PIN_MISO 50

/*
State machine related stuff
*/
enum State_enum { SAFE, IDLE, LOCOMOTION, HARVEST, STORE, CALIBRATE_SERVOS, TOTAL_STATES };
//enum Sensors_enum { NONE, SENSOR_RIGHT, SENSOR_LEFT, BOTH };



/*
Communication related stuff
*/
#define SERIAL_INPUT_SIZE 64


struct Messages
{
	enum Types_enum { HEARTBEAT, TELEMETRY, EMERGENCY_STOP, SET_MOTOR_SPEED, SET_SERVO_ANGLE, TOTAL_MESSAGE_TYPES};
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

