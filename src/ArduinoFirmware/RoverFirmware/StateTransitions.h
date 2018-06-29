#pragma once
#include "RoverControlModel.h"

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
	Serial.println("Transition -> IDLE");
	roverControlModel.DriveMotor0.targetSpeed = 0;
	roverControlModel.DriveMotor1.targetSpeed = 0;
	roverControlModel.DriveMotor2.targetSpeed = 0;
	roverControlModel.DriveMotor3.targetSpeed = 0;
	roverControlModel.MechanismMotor.targetSpeed = 0;
	roverControlModel.ArmServo0.angle = 0;
	roverControlModel.ArmServo1.angle = 0;
	roverControlModel.BucketServo.angle = 0;

}

void transition_locomotion()
{
	// + stow harvesting mechanism
	Serial.println("Transition -> LOCOMOTION");
	roverControlModel.ArmServo0.angle = 0;
	roverControlModel.ArmServo1.angle = 0;
	roverControlModel.BucketServo.angle = 0;
	roverControlModel.MechanismMotor.targetSpeed = 0;
}

void transition_harvest()
{
	// + update motor speeds
	// + update steering angles
	// + update harvesting mechanism
	Serial.println("Transition -> HARVEST");
	roverControlModel.ArmServo0.angle = 45;
	roverControlModel.ArmServo1.angle = 45;
	roverControlModel.BucketServo.angle = 0;
	roverControlModel.MechanismMotor.targetSpeed = 100;
}

void transition_store()
{
	// + set motor speed and steering to 0
	// + move mechanism to store position
	Serial.println("Transition -> STORE");
	roverControlModel.ArmServo0.angle = 0;
	roverControlModel.ArmServo1.angle = 0;
	roverControlModel.BucketServo.angle = 0;
	roverControlModel.MechanismMotor.targetSpeed = 0;
}
