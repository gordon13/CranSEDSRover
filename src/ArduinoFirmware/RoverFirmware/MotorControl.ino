#include "MotorControl.h"
#include "Constants.h"

int currentSpeed0;
void MotorControlSetup()
{
	currentSpeed0 = roverControlModel.DriveMotor0.speed;

	// set all the motor control pins to outputs
	pinMode(PIN_MOTOR0_ENA, OUTPUT);
	pinMode(PIN_MOTOR0_IN1, OUTPUT);
	pinMode(PIN_MOTOR0_IN2, OUTPUT);
	pinMode(PIN_MOTOR1_ENB, OUTPUT);
	pinMode(PIN_MOTOR1_IN3, OUTPUT);
	pinMode(PIN_MOTOR1_IN4, OUTPUT);
}

byte direction0 = NULL;
byte direction1 = NULL;
void MotorControlUpdate()
{
	if (currentSpeed0 != roverControlModel.DriveMotor0.speed) 
	{
		
	} 
	else 
	{
		direction0 = (roverControlModel.DriveMotor0.direction >= 1) ? HIGH : LOW;
		setPinValues(PIN_MOTOR0_IN1, direction0, PIN_MOTOR0_IN2, direction0 * -1, PIN_MOTOR0_ENA, roverControlModel.DriveMotor0.speed);
	}
	
	
	//direction1 = (roverControlModel.DriveMotor1.direction >= 1) ? HIGH : LOW;
	//setPinValues(PIN_MOTOR1_IN3, direction1, PIN_MOTOR1_IN4, direction1 * -1, PIN_MOTOR1_ENB, roverControlModel.DriveMotor1.speed);
}


void setPinValues(int ina, byte inaValue, 
				  int inb, byte inbValue, 
				  int en,  int speed) 
{
	digitalWrite(ina, inaValue);
	digitalWrite(inb, inbValue);
	analogWrite(en, speed);
}

/*
void demoTwo()
{
	// this function will run the motors across the range of possible speeds
	// note that maximum speed is determined by the motor itself and the operating voltage
	// the PWM values sent by analogWrite() are fractions of the maximum speed possible 
	// by your hardware
	// turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	// accelerate from zero to maximum speed
	for (int i = 0; i &lt; 256; i++)
	{
		analogWrite(enA, i);
		analogWrite(enB, i);
		delay(20);
	}
	// decelerate from maximum speed to zero
	for (int i = 255; i &gt; = 0; --i)
	{
		analogWrite(enA, i);
		analogWrite(enB, i);
		delay(20);
	}
	// now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
*/