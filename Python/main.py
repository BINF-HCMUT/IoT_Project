import  sys
import random
import time
from adafruit import *
from AI_Python import *
from timer import *
from uart import *

counter_ai = 60
counter_sensor = 60
setTimer(0, 1)
#setTimer(1, 1)
while True:
    # counter_sensor = counter_sensor -1
    # counter_ai = counter_ai - 1
    timerRun()
    # if timerTimeout(0):
    #     setTimer(0, 2)
    #     image_capture()
    #     ai_result = image_detector()
    #     client.publish("AI", ai_result)

    # if timerTimeout(1):
    #     setTimer(1, 3)
    #     temp = random.randint(0,100)
    #     humid = random.randint(0,100)
    #     print(temp," ",humid)
    #     client.publish("Temp_sensor", temp)
    #     client.publish("Humid_sensor", humid)
    if timerTimeout(0):
        setTimer(0, 10)
        writeSerial("RST")
    readSerial(client)
    time.sleep(1)