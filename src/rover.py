#!/usr/bin/python               #  This is server.py file
import socket                   # Import socket module
import time
import sys
from common import message_definitions as mdef

major, minor, micro, releaselevel, serial = sys.version_info
if (major, minor) < (3, 5):
    print("Python {}.{} is too low for this software. Please install Python 3.5 or higher".format(major, minor))
    sys.exit(2)

s = socket.socket()             # Create a socket object
host = socket.gethostname()     # Get local machine name
port = 12345                    # Reserve a port for your service.
s.bind(('', port))              # Bind to the port
s.listen(5)                     # Now wait for client connection.
while True:
    print('Rover initialised. Awaiting groundstation connection...')
    c, addr = s.accept()        # Establish connection with client.
    print('Got connection from', addr)
    c.send(b'Thank you for connecting')
    while True:
        time.sleep(0.05)  # This sets the base rate of update (0.1 second)
        data = c.recv(1024)
        if not data:
            break
        identifier = mdef.retrieve_data_identifier(data)
        if (identifier is not None and mdef.validate_identifier(identifier)):
            decoded_message = mdef.decode_message(data, identifier)

            if identifier == mdef.get_type_from_name("heartbeat"):
                print('heartbeat data{}'.format(decoded_message))
                # send heartbeat signal to the arduino, so it knows to keep the statemachine going
            elif identifier == mdef.get_type_from_name("emergency_stop"):
                print('stop{}'.format(decoded_message))
                # send stop signal to the arduino, so it knows to stop everything
            elif identifier == mdef.get_type_from_name("set_motor_speed"):
                print('set motor speed to {}'.format(decoded_message))
                # send motor speed update signal to the arduino, so it knows to update the speed of the motor
        else:
            print("Identifier not valid: {}".format(identifier))
            time.sleep(1)

    c.close()                   # Close the connection
