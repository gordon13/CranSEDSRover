#pragma once
#include "RoverControlModel.h"
#include "MotorControl.h"
#include "ServoControl.h"

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
	roverControlModel.ArmServo0.angle = 0;
	roverControlModel.ArmServo1.angle = 0;
	roverControlModel.MechanismServo.angle = 0;
	roverControlModel.DriveMotor0.targetSpeed = 0;
	roverControlModel.DriveMotor1.targetSpeed = 0;
	roverControlModel.DriveMotor2.targetSpeed = 0;
	roverControlModel.DriveMotor3.targetSpeed = 0;
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




