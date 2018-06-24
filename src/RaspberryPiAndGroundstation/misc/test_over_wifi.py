import socket
import sys
import spidev
import time
from common.message_definitions import *


def startComm():
    '''
    establishes first connection with the rover
    '''
    port = 12345
    try:
        s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    except socket.error:
        print("Failed to create socket")
        sys.exit() #quit program

    try:
        s.bind("",port) # "" = hostname, because we don't know the hostname yet. Receive from anyone
    except:
        print("Failed to bind")
        sys.exit()

    s.listen(5)                 #backlog i.e. the number of other clients that can wait in queue
    conn, address = s.accept()  #conn = connection object, addr = IP address of the client and Port
    data = conn.recv(1024)

    if not data:
        conn.send('no data,closing port')
        conn.close()
        s.close()
        return False
    else:
        data_decoded = decode_message(data)

    identifier = data_decoded[0]

    if identifier==5:
        setup_spi()
    else:
        conn.send('SPI start failed')
    conn.close()
    s.close()
    return True



def setup_spi():
    spi = spidev.SpiDev()  # create spi object
    spi.open(0, 0)  # open spi port 0, device (CS) 1
    spi.max_speed_hz = 500000
    spi.mode = 0b01


def recv_gs_send_spi():
    '''
        receives data from ground station and sends it to arduino
    '''
    port = 12345
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        print("Failed to create socket")
        sys.exit()  # quit program

    try:
        s.bind("", port)  # "" = hostname, because we don't know the hostname yet. Receive from anyone
    except:
        print("Failed to bind")
        sys.exit()

    s.listen(5)
    while True:
        conn, address = s.accept()  # conn = connection object, addr = IP address of the client and Port
        data = conn.recv(1024)
        if not data:
            break
        else:
            data_decoded = decode_message(data)

        identifier = data_decoded[0]
        command_name = get_name_from_identifier(identifier)

        #send command name and wait
        send_command_name = atod([command_name[0]]) #need to have unique first letter. No point sending the whole name.
        spi.xfer(send_command_name)
        time.sleep(0.001)

        #send command data and wait
        send_data = atod(list(data_decoded[1:-1]))
        spi.xfer(send_data)
        time.sleep(0.01)


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
            print(i)
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


def main():
    startComm()
    recv_gs_send_spi()

if __name__ == '__main__':
    main()
