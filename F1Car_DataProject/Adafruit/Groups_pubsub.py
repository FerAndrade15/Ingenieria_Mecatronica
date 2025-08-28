# Example of subscribing to an Adafruit IO group
# and publishing to the feeds within it

# Author: Brent Rubell for Adafruit Industries, 2018

# Import standard python modules.
import sys
import time
import serial

# Import Adafruit IO MQTT client.
from Adafruit_IO import MQTTClient

# hold the count for the feed
global run_count


# Set to the ID of the feed to suscribe to for updates.
feeddirection = 'Direction'
#feedspeed = 'Speed'
#feedlight = 'Light'
feedcabin = 'Cabin'
feedmode = 'Mode'
message = ''

# Define callback functions which will be called when certain events happen.
def connected(client):
    # Subscribe to changes on feeds
    client.subscribe(feeddirection)
    #client.subscribe(feedspeed)
    #client.subscribe(feedlight)
    client.subscribe(feedcabin)
    client.subscribe(feedmode)
    print('Waiting for feed data...')

def disconnected(client):
    # Disconnected function will be called when the client disconnects.
    print('Disconnected from Adafruit IO!')
    sys.exit(1)

def threebytesconvertion(content):
    if(int(content)<10):
        charvalue = "00"+content
    elif(int(content)<100):
        charvalue = "0"+content
    else:
        charvalue = content
    return charvalue

def message(client, feed_id, payload):
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    
    if(feed_id == feedmode):
        if(payload == '10'):
            print('Mode >> Manual')
            arduino.write(bytes('10', 'utf-8'))
        if(payload == '11'):
            print('Mode >> Eeprom')
            arduino.write(bytes('11', 'utf-8'))
        if(payload == '12'):
            print('Mode >> Uart')
            arduino.write(bytes('12', 'utf-8'))
    if(feed_id == feeddirection):
        servochar = threebytesconvertion(payload)
        arduino.write(bytes('20', 'utf-8'))
        arduino.write(bytes(servochar, 'utf-8'))  
    if(feed_id == feedcabin):
        servochar = threebytesconvertion(payload)
        arduino.write(bytes('23', 'utf-8'))
        arduino.write(bytes(servochar, 'utf-8'))  
try:
    # Create an MQTT client instance.
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Setup the callback functions defined above.
    client.on_connect    = connected
    client.on_disconnect = disconnected
    client.on_message    = message

    # Connect to the Adafruit IO server.
    client.connect()
    client.loop_background()
    
    arduino=serial.Serial(port='COM4', baudrate=9600, timeout=0.1)

    while True:
        message = arduino.readline().decode('utf-8')
        if(message == '10'):
            print('Mode>>Manual')
            client.publish(feedmode, 10)
        if(message == '11'):
            print('Mode>>Eeprom')
            client.publish(feedmode, 11)
        if(message == '12'):
            print('Mode>>Uart')
            client.publish(feedmode, 12)
        time.sleep(3)
        
except KeyboardInterrupt:
    print('Salimos del programa')
    if arduino.is_open:
        arduino.close()
    sys.exit(1)