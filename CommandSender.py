"""
Python command sender
"""
from struct import *
import socket
import sys
import re


commands = {
	"heartbeat": {
		"type":0,
		"bin_format": "h"
	},
	"emergency_stop":{
		"type":1,
		"bin_format": "h"
	},
	"set_motor_speed":{
		"type":2,
		"bin_format": "hf" # motor_index, motor_speed
	},
	"set_motor_steering":{
		"type":3,
		"bin_format": "hf" # motor_index, motor_speed
	}
}

#================
# raw_arguments = sys.argv

# arguments = []

# if (len(raw_arguments) > 1):
# 	arguments = raw_arguments[1:]

# print (arguments)

# if (len(arguments) < 1):
# 	print("No commands to send")
#============

def calculate_checksum(message):
	length = len(str(message))
	return length

def pack_message(command_name, message):
	if (command_name not in commands):
		print("Command not found")
	else:
		command = commands[command_name]
		formatter_string = command["bin_format"]
		message_type = command["type"]
		checksum = calculate_checksum(message)
		formatted_message = pack("h" + formatter_string + "l", message_type, *message, checksum)
		return formatted_message

#
# Sender code
#



s = socket.socket()
host = "192.168.4.1"
port = 12345

s.connect((host, port))
print(s.recv(1024))
s.send(pack_message("heartbeat", (0,)))
# s.send(pack_message("emergency_stop", (0,)))
# s.send(pack_message("set_motor_speed", (0, 54.2,)))
s.close