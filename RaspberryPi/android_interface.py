from bluetooth import *
from interface import *
from config import *
import os
import time


class android_interface(interface):
    def __init__(self):
        os.system('sudo hciconfig hci0 piscan')
        os.system('sudo systemctl start rfcomm')

    def connect(self, uuid):
        try:
            self.server_sock = BluetoothSocket(RFCOMM)
            print("Bluetooth Socket Created")

            self.server_sock.bind(("", 3))      #why is port 5?
            self.server_sock.listen(3)
            port = self.server_sock.getsockname()[1]
            print("Bluetooth Socket Bound")

            advertise_service(self.server_sock,
                              "SampleServer",
                              service_id=uuid,
                              service_classes=[uuid, SERIAL_PORT_CLASS],
                              profiles=[SERIAL_PORT_PROFILE], )

            print("Waiting for RFComm Channel %d..." %port)

            self.client_sock, self.client_info = self.server_sock.accept()
            secure = self.client_info[0]

            print("Connected to Bluetooth Device: %s.")# % str(self.client_info))

            return 1
        except Exception as e:
            print("Bluetooth Connection Failed: %s" % str(e))
            try:
                print("%s" % str(x))
                self.client_sock.close()
                self.server_sock.close()
            except:
                print("Error")
            return 0

    def disconnect(self):
        try:
            self.client_sock.close()
            self.server_sock.close()
        except Exception as e:
            print
            "Bluetooth disconnection exception: %s" % str(e)

    def reconnect(self):
        connected = 0
        connected = self.connect(unique_id)
        while connected == 0:
            print
            "Attempting reconnection..."
            # self.disconnect()
            time.sleep(1)
            connected = self.connect(unique_id)

    def write(self, msg):
        try:
            self.client_sock.send(msg)
        #          print "Write to Android: %s" %(msg)
        except Exception as e:
            print("Bluetooth write exception: %s" % str(e))
            self.reconnect()

    def read(self):
        try:
            msg = self.client_sock.recv(1024)
            #         print "Read from Android: %s" %(msg)
            return msg.decode("utf-8")
        except Exception as e:
            print("Bluetooth read exception: %s" % str(e))
            self.reconnect()

