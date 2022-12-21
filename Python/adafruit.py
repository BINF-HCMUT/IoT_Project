from Adafruit_IO import MQTTClient
from uart import writeSerial
AIO_FEED_ID = ["light-button","pump-button"]
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
    if feed_id == "light-button":
        writeSerial("BL:" + payload)
    if feed_id == "pump-button":
        writeSerial("BP:" + payload)


client = MQTTClient(AIO_USERNAME, AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()
