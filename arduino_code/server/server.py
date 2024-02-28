import sys

import serial
import time

arduino = serial.Serial(port=sys.argv[1], baudrate=115200, timeout=.01)


while True:
    command = input("enter a command: ")
    print("writing to arduino...")
    arduino.write(bytes(command, "ascii"))
    # while arduino.in_waiting:
    #     time.sleep(0.001)
    # return_message = arduino.readall()
    # print("return message: " + str(return_message, "ascii"))


