#include <SPI.h>
#include "RoverControlModel.h"

char buf[100];
//volatile byte command = 0;
volatile byte pos;
volatile bool process_it;
char motor_index[20];
char motor_speed[20];
char mechanism_angle[20];
char bucket_angle[20];
int i;

void SetupSPI(void)
{							
	// turn on SPI in slave mode
	SPCR |= bit(SPE);

	// have to send on master in, *slave out*
	pinMode(MISO, OUTPUT);
	//  pinMode (motor, OUTPUT);

	// get ready for an interrupt
	pos = 0;   // buffer empty
	process_it = false;

	// now turn on interrupts
	SPI.attachInterrupt();

}  // end of setup


ISR(SPI_STC_vect)
{
	byte c = SPDR;
	if (pos < sizeof buf) {
		buf[pos++] = c;
	}
	if (c == '\n') {
		process_it = true;
	}

}



void UpdateSPI(void)
{
	if (process_it)
	{
		if (buf[0] == '0') {
			// do operation
			Serial.println("heartbeat");
		}
		else if (buf[0] == '1') {
			// do operation
			Serial.println("telemetry");
		}
		else if (buf[0] == '2') {
			// do operation
			Serial.println("emergency stop");
			roverControlModel.DriveMotor0.targetSpeed = atoi(0);
			roverControlModel.DriveMotor1.targetSpeed = atoi(0);
			roverControlModel.DriveMotor2.targetSpeed = atoi(0);
			roverControlModel.DriveMotor3.targetSpeed = atoi(0);
			roverControlModel.MechanismMotor.targetSpeed = atoi(0);
			roverControlModel.state = SAFE;
		}
		else if (buf[0] == '3') {
			// do operation
      roverControlModel.state = LOCOMOTION;
      Serial.print("setting state to:");
      Serial.println(roverControlModel.state);
			motor_index[0] = buf[1];

			for (i = 2; i<5; i++) {
				motor_speed[i - 2] = buf[i];
			}
			
			switch (motor_index[0])
			{
			case '0':
				roverControlModel.DriveMotor0.targetSpeed = atoi(motor_speed);
				roverControlModel.DriveMotor0.direction = (atoi(motor_speed)) > 1 ? true : false;
				break;
			case '1':
				roverControlModel.DriveMotor1.targetSpeed = atoi(motor_speed);
				roverControlModel.DriveMotor1.direction = (atoi(motor_speed)) > 1 ? true : false;
				break;
			case '2':
				roverControlModel.DriveMotor2.targetSpeed = atoi(motor_speed);
				roverControlModel.DriveMotor2.direction = (atoi(motor_speed)) > 1 ? true : false;
				break;
			case '3':
				roverControlModel.DriveMotor3.targetSpeed = atoi(motor_speed);
				roverControlModel.DriveMotor3.direction = (atoi(motor_speed)) > 1 ? true : false;
				break;
			case '4':
				roverControlModel.MechanismMotor.targetSpeed = atoi(motor_speed);
				roverControlModel.MechanismMotor.direction = (atoi(motor_speed)) > 1 ? true : false;
				break;
			default:
				Serial.print("SPI - Invalid motor index :" + String(atoi(motor_index)));
				break;
			}

			Serial.print("select motor :");
			Serial.print(motor_index);
			Serial.print(",set speed to:");
			Serial.println(atoi(motor_speed));
		}
		else if (buf[0] == '4') {
			// do operation
			Serial.println("set motor steering");
		}
		else if (buf[0] == '5') {
			// do operation
			Serial.println("set mechanism angle");
			for (i = 2; i<5; i++) {
				mechanism_angle[i - 2] = buf[i];
			}
			roverControlModel.ArmServo0.angle = atoi(mechanism_angle);
			roverControlModel.ArmServo1.angle = atoi(mechanism_angle);
		}
		else if (buf[0] == '5') {
			// do operation
			Serial.println("set bucket angle");
			for (i = 2; i<5; i++) {
				bucket_angle[i - 2] = buf[i];
			}
			roverControlModel.BucketServo.angle = atoi(bucket_angle);
		}
		else {
			// do operation
			Serial.println("no command");
		}
		buf[pos] = 0;
		Serial.println(buf);
		pos = 0;
		process_it = false;
	}
}
