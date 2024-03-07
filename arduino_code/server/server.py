import sys

import serial
import time

arduino = serial.Serial(port=sys.argv[1], baudrate=115200, timeout=.01)

while True:
    command = input("enter a command: ")
    print("writing to arduino...")

    while (arduino.in_waiting):
        time.sleep(1)

    # while arduino.in_waiting:
    #     time.sleep(0.001)
    return_message = arduino.readall()
    print("return message: " + str(return_message, "ascii"))
    arduino.write(bytes(command, "ascii"))
