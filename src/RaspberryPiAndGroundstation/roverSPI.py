#!/usr/bin/python           # This is server.py file
from struct import *
import socket               # Import socket module
import spidev
import time
import common.message_definitions as md

spi = spidev.SpiDev() # create spi object
spi.open(0, 0) # open spi port 0, device (CS) 1
spi.max_speed_hz = 5000
spi.mode = 0b01

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = 12340                # Reserve a port for your service.
s.bind(('', port))        # Bind to the port
#messages =[{"bin_format":"h"}, {"bin_format":"h"}, {"bin_format":"hf"}, {"bin_format":"hf"}]
s.listen(5)                 # Now wait for client connection.
while True:
   c, addr = s.accept()     # Establish connection with client.
   print 'Got connection from', addr
   #c.send(b"Thank you for connecting")
   while True:
       data_recv = c.recv(1024)
       c.send(b"okay")
       print(md.decode_message(data_recv))
       data_decoded = list(md.decode_message(data_recv))
       data_send = md.atod(data_decoded[0:-1])
       spi.xfer(data_send)
       time.sleep(0.1)

        
c.close()                # Close the connectio

