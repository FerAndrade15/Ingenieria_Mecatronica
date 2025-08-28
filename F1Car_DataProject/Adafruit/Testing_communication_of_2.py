import serial
import time
transmiter=serial.Serial(port='COM4', baudrate=9600, timeout=0.1)
receiver=serial.Serial(port='COM5', baudrate=9600, timeout=0.1)
i = 0

def read():
    data = transmiter.readline()
    return data
def write(x):
    receiver.write(x)
    time.sleep(0.05)
    return
while True:
    content = read()
    if(content!=b''):
        write(content)
        i += 1
        print(str(i))