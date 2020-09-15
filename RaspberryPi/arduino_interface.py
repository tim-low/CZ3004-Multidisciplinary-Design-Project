from interface import *
from config import *
import time

import serial


class arduino_interface(interface):
    def __init__(self):
        self.port = serial_port
        self.baud = baudrate

    def connect(self):
        self.serial_socket = serial.Serial(self.port, self.baud, timeout=1)
        self.serial_socket.flush()

    def disconnect(self):
        self.serial_socket.close()

    def write(self, msg):
        self.serial_socket.write(str(msg).encode('utf-8'))
        #print("Write to Arduino: %s" % (msg))

    def read(self):
        msg = self.serial_socket.readline().decode('utf-8').rstrip()
        if msg:
            #print("Read from Arduino: %s\n" % msg)
            return msg
