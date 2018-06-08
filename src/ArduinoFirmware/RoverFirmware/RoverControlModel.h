#pragma once
#include "Constants.h"

/*
Rover model elements
*/
struct S_DriveMotor {
	int speed;
	int direction;
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
