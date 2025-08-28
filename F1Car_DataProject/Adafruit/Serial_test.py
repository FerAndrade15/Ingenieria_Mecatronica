import serial
import time
arduino=serial.Serial(port='COM4', baudrate=9600, timeout=0.1)
def read():
    data = arduino.readline()
    return data
def write(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    return
while True:
    print(read())
    time.sleep(1)