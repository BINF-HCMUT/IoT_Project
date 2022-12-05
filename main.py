import  sys
import random
import time
from Adafruit_IO import MQTTClient
from AI_Python import *


AIO_FEED_ID = ["Temp_button","Pump_button"]
AIO_USERNAME = "HCMUT_IOT"
AIO_KEY = "aio_IhZL44jpgMiZiuIxiV3Fe4ShZzVB"

def connected(client):
    print("Ket noi thanh cong ...")
    for id in AIO_FEED_ID:
        client.subscribe(id)

def subscribe(client , userdata , mid , granted_qos):
    print("Subscribe thanh cong ...")

def disconnected(client):
    print("Ngat ket noi ...")
    sys.exit(1)

def message(client , feed_id , payload):
    print("Data from " + feed_id + ":" + payload)


client = MQTTClient(AIO_USERNAME, AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()


counter_ai = 60
counter_sensor = 60
while True:
    counter_sensor = counter_sensor -1
    counter_ai = counter_ai - 1
    if counter_ai <= 0:
        counter_ai = 60
        image_capture()
        ai_result = image_detector()
        client.publish("AI", ai_result)

    if counter_sensor <= 0:
        counter_sensor = 60
        temp = random.randint(0,100)
        humid = random.randint(0,100)
        print(temp," ",humid)
        client.publish("Temp_sensor", temp)
        client.publish("Humid_sensor", humid)
    time.sleep(1)