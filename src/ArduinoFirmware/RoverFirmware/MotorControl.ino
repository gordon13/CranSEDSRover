#include "Constants.h"
#include "SigmoidLUT.h"

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

int currentSpeed0;
int currentSpeed1;
byte direction0;
byte direction1;
int dt;
void MotorControlSetup(int updateInterval)
{
	currentSpeed0 = roverControlModel.DriveMotor0.targetSpeed;
	direction0 = roverControlModel.DriveMotor0.direction;
	dt = updateInterval;

	// set all the motor control pins to outputs
	pinMode(PIN_MOTOR0_ENA, OUTPUT);
	pinMode(PIN_MOTOR0_IN1, OUTPUT);
	pinMode(PIN_MOTOR0_IN2, OUTPUT);
	pinMode(PIN_MOTOR1_ENB, OUTPUT);
	pinMode(PIN_MOTOR1_IN3, OUTPUT);
	pinMode(PIN_MOTOR1_IN4, OUTPUT);
}


int accelerationtime = 1000;
int acceleration_iterations = int(accelerationtime / dt);
int number_lut_points = arr_len(SigmoidLUT);
int sample_every = int(number_lut_points / acceleration_iterations);
int newspeed0 = roverControlModel.DriveMotor0.currentSpeed;
int sigmoid_i0 = 0;
int speed_difference0 = roverControlModel.DriveMotor0.targetSpeed - roverControlModel.DriveMotor0.currentSpeed;

void MotorControlUpdate()
{
	if (currentSpeed0 != roverControlModel.DriveMotor0.targetSpeed)
	{
		newspeed0 = roverControlModel.DriveMotor0.currentSpeed + speed_difference0 * SigmoidLUT[sigmoid_i0];
		if (sigmoid_i0 + sample_every < number_lut_points)
		{
			sigmoid_i0 += sample_every;
			roverControlModel.DriveMotor0.currentSpeed = newspeed0;
		}
	} 
	else 
	{
		direction0 = (roverControlModel.DriveMotor0.direction >= 1) ? HIGH : LOW;
		setPinValues(PIN_MOTOR0_IN1, direction0, PIN_MOTOR0_IN2, direction0 * -1, PIN_MOTOR0_ENA, roverControlModel.DriveMotor0.targetSpeed);
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