from pc_interface import *
from arduino_interface import *
import threading
import time
from collections import deque

class Main:
    def __init__(self):
        #Initialize Rpi-specific connections


        #Initialize Connections to Hardware
        self.pc = pc_interface()
        self.pc.connect()

        self.arduino = arduino_interface()
        self.arduino.connect()

        self.android = android_interface()
        self.android.connect(unique_id)

        #Initialize Queues?
        self.serQ = deque([])  # queue for arduino
        self.btQ = deque([])  # queue for bluetooth
        self.wifiQ = deque([])  # queue for laptop
        self.delay = 0.0

        print("=============================")
        print("=====Communication Start=====")
        print("=============================")


    def readPC(self):
        while True:
            msg = self.pc.read()
            if msg:
                #self.wifiQ.append(msg)
                #self.btQ.append(msg)
                self.serQ.append(msg)
            time.sleep(self.delay)

    def writePC(self):
        while True:
            time.sleep(self.delay)
            if len(self.wifiQ) > 0:
                msg = self.wifiQ.popleft()
                if msg:
                    self.pc.write(msg)
                    print("Write to PC: %s\n" % msg)

    def readArduino(self):
        while True:
            msg = self.arduino.read()
            if (msg!=''):
                #self.btQ.append(msg)
                self.btQ.append(msg)
            time.sleep(self.delay)
            #print(msg)
          #  if msg:

            #TODO: Change to appropriate queue/WRITEDEVICE()
            #self.writeArduino(msg)
           #     self.writePC(msg)

    def writeArduino(self):
        while True:
            time.sleep(self.delay)
            if len(self.serQ) > 0:
                msg = self.serQ.popleft()
                if msg:
                    self.arduino.write(msg+"\n") #+\n
                    print("writeArduino has written")
                    if (msg == "Z"):    #end thread when at goal
                        break
        print("Arduino has reached the goal.")


    def readAndroid(self):
        while True:
            msg = self.android.read()
            if msg:
                self.wifiQ.append(msg)
            time.sleep(self.delay)

    def writeAndroid(self, msg):
        while True:
            time.sleep(self.delay)
            if len(self.btQ) > 0:
                msg = self.serQ.popleft()
                if msg:
                    self.android.write(msg)  # +\n
                    print("writeAndroid has written")


    def multithread(self):
        try:
            read_pc_thread = threading.Thread(target=self.readPC, name = "read_pc_thread")
            write_pc_thread = threading.Thread(target=self.writePC, name = "write_pc_thread")
            read_arduino_thread = threading.Thread(target=self.readArduino, name = "read_arduino_thread")
            write_arduino_thread = threading.Thread(target=self.writeArduino, name = "write_arduino_thread")
            read_android_thread = threading.Thread(target=self.readAndroid, name = "read_android_thread")
            write_android_thread = threading.Thread(target=self.writeAndroid, name = "write_android_thread")

            read_pc_thread.daemon=True
            write_pc_thread.daemon=True
            read_arduino_thread.daemon=True
            write_arduino_thread.daemon=True
            read_android_thread.daemon=True
            write_android_thread.daemon=True

            read_pc_thread.start()
            write_pc_thread.start()
            read_arduino_thread.start()
            write_arduino_thread.start()
            read_android_thread.start()
            write_android_thread.start()

            read_pc_thread.join()
            write_pc_thread.join()
            read_arduino_thread.join()
            write_arduino_thread.join()
            read_android_thread.join()
            write_android_thread.join()

            while(1):
                pass

        except Exception as e:
            print("Error in threading: %s" % str(e))

        while(1):
            pass

try:
        test = Main()
        test.multithread()
except KeyboardInterrupt:
        print("Terminating the program now...")
