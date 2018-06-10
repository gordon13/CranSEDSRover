#pragma once

/*
Serial input
Commands:
- read [propertytype] [subproperty] [subproperty 2] ... [subproperty n]
- write [propertytype] [subproperty] [subproperty 2] ... [subproperty n] [value]
*/
String command = "";
String argument1 = "";
void UpdateDebugConsole() {
	if (Serial.available() > 0 && Serial.available() <= 64)
	{
		// Get next command from Serial (add 1 for final 0)
		char input[SERIAL_INPUT_SIZE + 1];
		byte size = Serial.readBytes(input, SERIAL_INPUT_SIZE);
			
		// Add the final 0 to end the C string
		input[size] = 0;

		// Read the first part of the received string up to the delimiter
		char* command = strtok(input, " ");
		while (command != NULL)
		{
			Serial.println("command:" + String(command));
			if (strcmp(command, "read") == 0) // first part should be a command
			{
				char* propertytype = strtok(NULL, " "); // Read the next parameter in the original string we received
				Serial.println("cmdtype:" + String(propertytype));

				if (strcmp(propertytype, "roverstate") == 0) // This part should be a property type
				{
					char* prop = strtok(NULL, " ");
					Serial.println("prop:" + String(prop)); // Again, read the next parameter available

					if (strcmp(prop, "state") == 0) // This part should be a sub property
					{
						Serial.println("OUT: State:" + String(roverControlModel.state));
						break;
					}
					else if (strcmp(prop, "steeringservo0") == 0)
					{
						char* propspecific = strtok(NULL, " ");
						if (strcmp(propspecific, "angle") == 0)
						{
							Serial.println("OUT: Angle:" + String(roverControlModel.SteeringServo0.angle));
							break;
						}
						else if (strcmp(propspecific, "minpulse") == 0)
						{
							Serial.println("OUT: minpulse:" + String(roverControlModel.SteeringServo0.minpulse));
							break;
						}
						else if (strcmp(propspecific, "maxpulse") == 0)
						{
							Serial.println("OUT: maxpulse:" + String(roverControlModel.SteeringServo0.maxpulse));
							break;
						}
						else if (strcmp(propspecific, "zerocorrectionpulse") == 0)
						{
							Serial.println("OUT: zerocorrectionpulse:" + String(roverControlModel.SteeringServo0.zerocorrectionpulse));
							break;
						}
						else if (strcmp(propspecific, "midpulse") == 0)
						{
							Serial.println("OUT: midpulse:" + String(roverControlModel.SteeringServo0.midpulse()));
							break;
						}
						else
						{
							Serial.println("OUT: '" + String(propspecific) + "' not a valid rover state property.");
							break;
						}
					}
					else if (strcmp(prop, "drivemotor0") == 0)
					{
						char* propspecific = strtok(NULL, " ");
						if (strcmp(propspecific, "currentspeed") == 0)
						{
							Serial.println("OUT: currentspeed:" + String(roverControlModel.DriveMotor0.getCurrentSpeed()));
							break;
						}
						else if (strcmp(propspecific, "targetspeed") == 0)
						{
							Serial.println("OUT: targetspeed:" + String(roverControlModel.DriveMotor0.targetSpeed));
							break;
						}
						break;
					}
					else
					{
						Serial.println("OUT: '" + String(prop) + "' not a valid rover state property.");
						break;
					}
				}
			}
			else if (strcmp(command, "write") == 0)
			{
				char* propertytype = strtok(NULL, " ");
				Serial.println("cmdtype:" + String(propertytype));

				if (strcmp(propertytype, "roverstate") == 0)
				{
					char* prop = strtok(NULL, " ");
					Serial.println("prop:" + String(prop));

					if (strcmp(prop, "state") == 0)
					{
						char* propvalue = strtok(NULL, " ");
						Serial.println("propvalue:" + String(propvalue));

						int stateindex;
						if (validatePropValueInt(propvalue))
						{
							int stateindex = atoi(propvalue);
							if (stateindex >= 0 || stateindex < TOTAL_STATES) {
								roverControlModel.state = static_cast<State_enum>(stateindex);
								Serial.println("OUT: Set state to " + String(roverControlModel.state));
								break;
							}
							else
							{
								Serial.println("OUT: '" + String(stateindex) + "' not a valid state index.");
								break;
							}
						}
						else
						{
							Serial.println("OUT: '" + String(stateindex) + "' state index should be a number.");
							break;
						}
					}
					else if (strcmp(prop, "drivemotor0") == 0)
					{
						char* specificprop = strtok(NULL, " ");
						Serial.println("specificprop:" + String(specificprop));

						if (strcmp(specificprop, "targetspeed") == 0)
						{
							char* propvalue = strtok(NULL, " ");
							Serial.println("propvalue:" + String(propvalue));

							if (validatePropValueInt(propvalue))
							{
								int speed = atoi(propvalue);
								if (speed >= 0 && speed < 256) {
									roverControlModel.DriveMotor0.targetSpeed = speed;
									Serial.println("OUT: Set speed to " + String(speed));
									command = NULL;
									break;
								}
								else
								{
									Serial.println("OUT: '" + String(speed) + "' speed should be positive and less than 256.");
									command = NULL;
									break;
								}
							}
						}
					}
				}
				else
				{
					Serial.println("OUT: '" + String(propertytype) + "' not a valid rover property.");
					break;
				}
			}
			else
			{
				Serial.println("OUT: '" + String(command) + "' not a valid command.");
				break;
			}
			// Find the next command in input string
			//command = strtok(0, " ");
			break;
		}
	}
	else if (Serial.available() > 64)
	{
		Serial.println("OUT: Command is too long. Should be less than 64 bytes!");
	}
}

