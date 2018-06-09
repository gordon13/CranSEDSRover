#include <dht.h>
#include "Constants.h"


dht DHT;

void BatteryControlSetup()
{
	pinMode(PIN_SENSOR_VOLTAGE, INPUT);
}

void BatteryControlUpdate()
{
	roverControlModel.BatteryVoltage = analogRead(PIN_SENSOR_VOLTAGE);

	int chk = DHT.read(PIN_SENSOR_TEMPERATURE);
	roverControlModel.BatteryTemperature = DHT.temperature;
	roverControlModel.BatteryHumidity = DHT.humidity;
}