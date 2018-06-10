#include "Constants.h"
#include "SigmoidLUT.h"

void MotorControlSetup()
{
	// set all the motor control pins to outputs
	pinMode(PIN_MOTOR0_ENA, OUTPUT);
	pinMode(PIN_MOTOR0_IN1, OUTPUT);
	pinMode(PIN_MOTOR0_IN2, OUTPUT);
	pinMode(PIN_MOTOR1_ENB, OUTPUT);
	pinMode(PIN_MOTOR1_IN3, OUTPUT);
	pinMode(PIN_MOTOR1_IN4, OUTPUT);
}


void MotorControlUpdate()
{
	updateMotorPinValues(roverControlModel.DriveMotor0);
	updateMotorPinValues(roverControlModel.DriveMotor1);
}

void updateMotorPinValues(S_DriveMotor &motor) 
{	
	int target = abs(motor.targetSpeed);
	if (target != motor.getCurrentSpeed())
	{
		int difference = target - motor.getCurrentSpeed();
		if (difference >= 1 && motor.getCurrentSpeed() < target)
		{
			motor.incrementSpeed(motor.accelerationIncrement);
		}
		else if (difference < 1 && motor.getCurrentSpeed() > target)
		{
			motor.incrementSpeed(-motor.accelerationIncrement);
		}
		analogWrite(motor.pin_enable, motor.getCurrentSpeed());
	} 
	else 
	{
		analogWrite(motor.pin_enable, target);
	}
	digitalWrite(motor.pin_ina, (motor.direction >= 1) ? HIGH : LOW);
	digitalWrite(motor.pin_inb, (motor.direction >= 1) ? LOW : HIGH);	
}