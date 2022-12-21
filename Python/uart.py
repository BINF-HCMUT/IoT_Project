print("Hellow Sensors")
import serial.tools.list_ports
import time
#from adafruit import *

def getPort():
    ports = serial.tools.list_ports.comports()
    N = len(ports)
    commPort = "None"
    for i in range(0, N):
        port = ports[i]
        strPort = str(port)
        if "COM11" in strPort: #tùy theo tên port trên PC
            splitPort = strPort.split(" ")
            commPort = (splitPort[0])
            #print(commPort)
    return commPort

portName = getPort()

if portName != "None":
    ser = serial.Serial(port=portName, baudrate = 9600)
    print(ser)
else:
    print("NONE")

mess = ""
#receive_ready = 0

def processData(data, client):
    data = data.replace("!", "")
    data = data.replace("#", "")
    global splitData
    splitData = data.split(":")
    print(splitData)
    try:
        if splitData[0] == "HUMID":
            print("Cap nhat HUMID qua sensor: ", round((int(splitData[1]) / 4096) * 100, 2))
            client.publish("humid-sensor", round((int(splitData[1]) / 4096) * 100, 2))
            writeSerial("OK")
        elif splitData[0] == "TEMP":
            print("Cap nhat TEMP qua sensor: ", round((int(splitData[1]) / 4096) * 100, 2))
            client.publish("temp-sensor", round((int(splitData[1]) / 4096) * 100, 2))
        # elif splitData[0] == "CAMBIEN3":
        #     print("Cap nhat cambien3 qua sensor: ", splitData[1])
        #     client.publish("cambien3", splitData[1])
        #elif splitData[0] == "RECEIVER":
        #    global receive_ready
        #    receive_ready = splitData[1]
    except:
        pass

def readSerial(client):
    bytesToRead = ser.inWaiting()
    data = ""
    if (bytesToRead > 0):
        global mess
        mess = mess + ser.read(bytesToRead).decode("UTF-8")
        while ("#" in mess) and ("!" in mess):
            start = mess.find("!")
            end = mess.find("#")
            processData(mess[start:end + 1], client)
            if (end == len(mess)):
                mess = ""
            else:
                mess = mess[end + 1:]
        #writeSerial("OK")

def writeSerial(value):
    write_data = "!" + str(value) + "#"
    #if receive_ready == 1:
    ser.write(write_data.encode())

