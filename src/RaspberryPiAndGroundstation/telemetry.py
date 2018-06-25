#!/usr/bin/python           # This is server.py file
from struct import *
import socket               # Import socket module

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = 12345                # Reserve a port for your service.
s.bind(('', port))        # Bind to the port
#messages =[{"bin_format":"h"}, {"bin_format":"h"}, {"bin_format":"hf"}, {"bin_format":"hf"}]
s.listen(5)                 # Now wait for client connection.
while True:
   c, addr = s.accept()     # Establish connection with client.
   print 'Got connection from', addr
   #c.send(b"Thank you for connecting")
   while True:
       data = c.recv(1024)
       c.send(b"okay")
       print(data)
        
   c.close()                # Close the connectio
