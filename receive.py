#!/usr/bin/python           # This is server.py file
from struct import *
import socket               # Import socket module

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = 12345                # Reserve a port for your service.
s.bind(('', port))        # Bind to the port
messages =[{"bin_format":"h"}, {"bin_format":"h"}, {"bin_format":"hf"}, {"bin_format":"hf"}]
s.listen(5)                 # Now wait for client connection.
while True:
   c, addr = s.accept()     # Establish connection with client.
   print 'Got connection from', addr
   c.send('Thank you for connecting')
   while True:
       data = c.recv(1024)
       print(data)
       identifier = unpack('h',data[0:2])[0]
       print(identifier)
       formatR = messages[identifier]["bin_format"]
       stringrec = unpack('h'+formatR+'l',data)
       if identifier==0:        
           print('heartbeat data{}'.format(stringrec))
       elif identifier==1:
           print('stop{}'.format(stringrec))
       elif identifier==2:
           print('set motor speed to {}'.format(stringrec))
       else:
           print('this is not a format')

        
   c.close()                # Close the connectio