#pragma once
#include "Constants.h"

/*
Rover model elements
*/
struct S_DriveMotor {
	int targetSpeed;
	int direction;
	float accelerationTime;

	// used by the driver
	int pin_ina; 
	int pin_inb;
	int pin_enable;
	int getCurrentSpeed() 
	{
		return currentSpeed;
	}
	void incrementSpeed(int value) 
	{
		Serial.println("value: " + String(value) + ", current speed: " + String(currentSpeed));
		int incval = currentSpeed + value;
		currentSpeed = constrain(incval, 0, 255);
		Serial.println("value: " + String(value) + ", current speed: " + String(currentSpeed));
	}
private:
	// shouldn't be able to edit this directly
	int currentSpeed;
};

struct S_ServoMotor {
	float angle;
	int minpulse;
	int maxpulse;
	int zerocorrectionpulse;
	int midpulse() {
		return (minpulse + maxpulse) / 2 + zerocorrectionpulse;
	}
};




/*
Main rover model
*/
struct S_RoverControlModel {
	State_enum state;  // time
	long time_since_heartbeat;  // time

	// servos
	S_ServoMotor SteeringServo0;
	S_ServoMotor SteeringServo1;

	// drive motors
	S_DriveMotor DriveMotor0;  // RPM
	S_DriveMotor DriveMotor1;  // RPM

	// sensors
	float BatteryTemperature; // Temp
	float BatteryVoltage;  // Voltage
	float BatteryHumidity;  // Humidity

	S_ServoMotor getServoByIndex(int i) {
		switch (i) {
		case 0:
			return SteeringServo0;
			break;
		case 1:
			return SteeringServo1;
			break;
		}
	}
} roverControlModel;
