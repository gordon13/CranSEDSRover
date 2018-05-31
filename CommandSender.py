"""
Python message sender sender
"""
from struct import *
import socket
import time
import sys
import re

import message_definitions as mdef

#================
# raw_arguments = sys.argv

# arguments = []

# if (len(raw_arguments) > 1):
# 	arguments = raw_arguments[1:]

# print (arguments)

# if (len(arguments) < 1):
# 	print("No commands to send")
#============

def server_message(message):
	return "SERVER: {}".format(message)

def calculate_checksum(message):
	length = len(str(message))
	return length

""" 
# h: first byte. used for determining the type of message sent/received
# middle part: determined by the message format
# l: last byte. used for error checking """
def create_format_string(message_format):
	return "h" + message_format + "l"

def pack_message(message_name, data):
	if (message_name not in mdef.messages):
		print("Message not found")
	else:
		current_message_def = mdef.get_by_name(message_name)
		format_string = create_format_string(current_message_def["bin_format"])
		checksum = calculate_checksum(data)
		formatted_message = pack(format_string, current_message_def["type"], *data, checksum)
		return formatted_message

#
# Sender code
#
host = "192.168.4.1"
port = 12345

s = socket.socket()
s.settimeout(3) # timeout after 3 seconds of trying to connect

print("Connecting to {}:{} ...".format(host, port))
s.connect((host, port))

print("Connected.")
print(server_message(s.recv(1024)))

tCurrent10 = time.time() # keep track of time every 10 second
tCurrent1 = time.time() # keep track of time every 1 second
tCurrent01 = time.time() # keep track of time every 0.1 second

"""
This loop keeps track of different timings. There is a base 0.1 second delay for each iteration (this seems good enough for a groundstation).
There are multiple if statements that do different things at different times. 

For example:
- The "heartbeat" is sent every second. This is an example of something that needs to happen automatically all the time.
- The emergency stop is sent every 10 seconds, however this is just an example. In the final code, this will be manually controlled by a button.
- The set motor speed is sent every 0.1 seconds to simulate theupdating the speed of the rover very often. In the final code, this function will either be a button, or will be connected to a joystick, so we need the ability to send this comment very quickly """
while True:
    time.sleep(0.1) # This sets the base rate of update (0.1 second)

    if time.time() >= tCurrent10 + 10: # this updates something every 10 seconds
        print("Stop")
        s.send(pack_message("emergency_stop", (0,)))
        tCurrent10 = time.time()

    if time.time() >= tCurrent1 + 1: # this updates something every second
    	print(heartbeat)
    	s.send(pack_message("heartbeat", (0,)))
    	tCurrent1 = time.time()

    if time.time() >= tCurrent01 + 0.1: # this updates something every 0.1 seconds
    	print("Set motor speed")
    	s.send(pack_message("set_motor_speed", (0, 54.2,)))
    	tCurrent01 = time.time()

s.close