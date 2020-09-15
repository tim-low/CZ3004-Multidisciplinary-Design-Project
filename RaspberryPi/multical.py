
from pc_interface import *
from arduino_interface import *
import threading
import Queue


class Main:
    def __init__(self):
        self.pc = pc_interface()
        self.pc.connect()

        self.arduino = arduino_interface()
        self.arduino.connect()

        print("=============================")
        print("=====Communication Start=====")
        print("=============================")

    # PC write to RPi
    # RPi write to Arduino
    # Arduino write back to RPi

    def readPc(self):
        while True:
            msg = self.pc.read()
            if msg:
                self.writeArduino(msg)

    def writePc(self, msg):
        if msg:
            self.pc.write(msg)

    def readArduino(self):
        while True:
            msg = self.arduino.read()

        # while True:
        #     msg = self.arduino.read()
        #     self.writeArduino(msg)
            # print(msg)
          #  if msg:

            # TODO: Change to appropriate queue/WRITEDEVICE()
            # self.writeArduino(msg)
           #     self.writePC(msg)

    def writeArduino(self, msg):
        if msg:
            self.arduino.write(msg)

    def readAndroid(self):
        while True:
            pass

    def writeAndroid(self, msg):
        pass

    def multithread(self):
        try:
            read_pc_thread = threading.Thread(
                target=self.readPc, name="read_pc_thread")
            read_arduino_thread = threading.Thread(
                target=self.readArduino, name="read_arduino_thread")

            read_pc_thread.start()
            read_arduino_thread.start()

            read_pc_thread.join()
            read_arduino_thread.join()

            while 1:
                pass
        except KeyboardInterrupt:
                print("Terminating program")
        except Exception as e:
            print("Error in threading: %s" % str(e))


try:
    test = Main()
    test.multithread()

except KeyboardInterrupt:
    print("Terminating the program now...")

