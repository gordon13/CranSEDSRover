#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "RoverControlModel.h"
#include "Constants.h"
#include "Utility.h"


/*
Global variables
*/
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


/*
Main functions
*/
void ServoControlSetup() {
	Serial.println("Servo controller setup");
	pwm.begin();
	pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}


void ServoControlUpdate() {
	pwm.setPWM(0, 0, degToPulse(roverControlModel.ArmServo0.angle));
}


void ServoControlCalibrateUpdate() {
	Serial.println("Servo controller calibrate update \n");
	
	calibrateServo(0);
	calibrateServo(1);
}

void calibrateServo(int index) {
	S_ServoMotor servo = roverControlModel.getServoByIndex(index);
	Serial.println("\n\nServo: " + String(index));
	Serial.println("Calibrate servo centers:");
	pwm.setPWM(index, 0, servo.midpulse());
	Serial.println("Does this value result in servos pointing straight ahead?");
	delay(1000);


	Serial.println("Calibrate servo MAX:");
	for (uint16_t pulselen = servo.minpulse; pulselen < servo.maxpulse; pulselen++) { // TODO - replace
		pwm.setPWM(index, 0, pulselen);
		Serial.println("pulse length:" + String(pulselen));
	}
	delay(500); // TODO - replace


	Serial.println("Calibrate servo MIN:");
	for (uint16_t pulselen = servo.maxpulse; pulselen > servo.minpulse; pulselen--) { // TODO - replace
		pwm.setPWM(index, 0, pulselen);
		Serial.println("pulse length:" + String(pulselen));
	}
	delay(500); // TODO - replace


	Serial.println("Again, calibrate servo centers:");
	pwm.setPWM(index, 0, servo.midpulse());
	Serial.println("Does this value result in servos pointing straight ahead?");
	delay(2000); // TODO - replace
}