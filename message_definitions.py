"""
This file contains an array with all the messages we may need
"""

messages = [
	{
		"name": "heartbeat",
		"type":0,
		"bin_format": "h",
		"description": "heartbeat"
	},
	{
		"name": "emergency_stop",
		"type":1,
		"bin_format": "h",
		"description": "emergency_stop"
	},
	{
		"name": "set_motor_speed",
		"type":2,
		"bin_format": "hf",
		"description": "set_motor_speed: motor_index, motor_speed"
	},
	{
		"name": "set_motor_steering",
		"type":3,
		"bin_format": "hf",
		"description": "set_motor_steering: motor_index, motor_speed"
	}
] 

def get_by_name(message_name):
	for a in messages:
		if (a["name"] == message_name):
			return a
	raise "{} not found in message definition list".format(message_name)