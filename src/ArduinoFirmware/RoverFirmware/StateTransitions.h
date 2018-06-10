#pragma once

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
	roverControlModel.DriveMotor0.targetSpeed = 0;
	//roverControlModel.SteeringServo0Angle = 0;
	Serial.println("Transition -> IDLE");
}

void transition_locomotion()
{
	// + stow harvesting mechanism
	Serial.println("Transition -> LOCOMOTION");

}

void transition_harvest()
{
	// + update motor speeds
	// + update steering angles
	// + update harvesting mechanism
	Serial.println("Transition -> HARVEST");
}

void transition_store()
{
	// + set motor speed and steering to 0
	// + move mechanism to store position
	Serial.println("Transition -> STORE");
}
