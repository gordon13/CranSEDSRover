void BatteryControlSetup()
{
	// set all the motor control pins to outputs
	pinMode(PIN_SENSOR_TEMPERATURE, INPUT);
	pinMode(PIN_SENSOR_VOLTAGE, INPUT);
}

void BatteryControlUpdate()
{
	drive(0, roverControlModel.DriveMotor0.direction, roverControlModel.DriveMotor0.speed)
		drive(1, roverControlModel.DriveMotor1.direction, roverControlModel.DriveMotor1.speed)
}