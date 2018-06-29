#pragma once
#include "Constants.h"

/*
Rover model elements
*/
struct S_DriveMotor {
	int targetSpeed;
	int direction;
	float accelerationIncrement;

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
		currentSpeed = constrain(currentSpeed + value, 0, 255);
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
	S_ServoMotor ArmServo0;
	S_ServoMotor ArmServo1;
	S_ServoMotor BucketServo;

	// drive motors
	S_DriveMotor DriveMotor0;  // RPM
	S_DriveMotor DriveMotor1;  // RPM
	S_DriveMotor DriveMotor2;  // RPM
	S_DriveMotor DriveMotor3;  // RPM
	S_DriveMotor MechanismMotor;  // RPM

	// sensors
	float BatteryTemperature; // Temp
	float BatteryVoltage;  // Voltage
	float BatteryHumidity;  // Humidity

	S_ServoMotor getServoByIndex(int i) {
		switch (i) {
		case 0:
			return ArmServo0;
			break;
		case 1:
			return ArmServo1;
			break;
		case 2:
			return BucketServo;
			break;
		}
	}
} roverControlModel;
