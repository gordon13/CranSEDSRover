#!/usr/bin/env python
from struct import *
import spidev
import time

spi = spidev.SpiDev() # create spi object
spi.open(0, 0) # open spi port 0, device (CS) 1
spi.max_speed_hz = 5000
spi.mode = 0b01


def transferWait(data):
    spi.xfer(data)
    time.sleep(0.1)

def atod(ascii):
    '''
        takes in ascii value and converts to decimal equivalent
    '''
    decimal = []
    for i in ascii:
        if isinstance(i,str):
            value = ord(i)
            decimal.append(value)
        else:
            i = int(i)
            number = str(i)
            value_local = ""
            if len(number)>1:
                for j in number:
                    value = ord(str(j))
                    decimal.append(value)
            else:
                value = ord(str(i))
                decimal.append(value)

    decimal.append(10)

    return decimal

try:
    while True:
        data = atod([3,3,100])
        #print(data)
        spi.xfer(data)
        time.sleep(0.01)
        spi.xfer(atod([3,2,200]))
        time.sleep(0.01)
        #transferWait(data)
        #data = [3]
        #spi.xfer(data)
        #time.sleep(0.1)
        #spi.no_cs= True
        #x = spi.readbytes(1)
        #print(x[0])
        #print(resp)
        #time.sleep(0.1) # sleep for 0.1 seconds
except KeyboardInterrupt:
    print("stopped by user")
    spi.close()

