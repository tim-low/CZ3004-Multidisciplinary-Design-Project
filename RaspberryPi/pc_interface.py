import sys
import traceback
import errno

import socket
from interface import *
from config import *

# wifi_ip_address = '192.168.32.32'
# wifi_port = 5050

class pc_interface(interface):

    def __init__(self):
        self.host = wifi_ip_address
        self.port = wifi_port


    def connect(self):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        print("PC Socket Created")

        try:
            self.server_socket.bind((self.host, self.port))
        except socket.error as e:
            print("PC Socket Bind failed: ", e)
            sys.exit()

        print("PC Socket Bound")

        self.server_socket.listen(5)
        print("Waiting for connection from PC...")

        self.client_socket, self.address = self.server_socket.accept()
        print("Connected to PC @ ", self.address, "!")


    def disconnect(self):
        try:
            self.server_socket.close()

        except Exception as e:
            print("PC Socket Disconnect failed: %s" % str(e))


    def write(self, msg):
        try:
            self.client_socket.sendto(msg.encode('utf-8'), self.address)

        except socket.error as e:
            if isinstance(e.args, tuple):
                print("Error No.: %d" % e[0])
                if e[0] == errno.EPIPE:
                    # remote peer disconnected
                    print("Remote Host Disconnected")
                else:
                    print("Socket Error: ", e)
                sys.exit()

        except IOError as e:
            print("PC Read Failed: ", e)
            print(traceback.format_exc())

        #print("Writing to PC: %s" % (msg))


    def read(self):
        try:
            msg = self.client_socket.recv(1024)
            if msg:
                #print("Read from PC: %s" % (msg.decode('utf-8')))
                return msg.decode('utf-8')

        except socket.error as e:
            if isinstance(e.args, tuple):
                print("Error No.: %d" % e[0])
                if e[0] == errno.EPIPE:
                    print("Remote Host Disconnected")

            else:
                print("Socket Error: ", e)
            sys.exit()


        except IOError as e:
            print("PC Read Failed: ", e)
            print(traceback.format_exc())

