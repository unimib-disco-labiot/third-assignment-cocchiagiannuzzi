from flask_mqtt import Mqtt
from flask import current_app as app

class MqttHandler:
    def __init__(self):
        self.mqtt = Mqtt(app)

        def subscribe(topic, qos=0):
            self.mqtt.subscribe(topic, qos)

        def publish(topic, message):
            self.mqtt.publish(topic, message)

        @self.mqtt.on_connect()
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected with result code " + str(rc), end="\n\n\n")
                self.subscribe("cocchiagiannuzzi/test")
            else:
                print("Bad connection. Returned code=", rc)

        @self.mqtt.on_disconnect()
        def on_disconnect(client, userdata, rc):
            print("disconnecting reason  " + str(rc))

        @self.mqtt.on_message()
        def on_message(client, userdata, message):
            data = dict(
                topic=message.topic,
                payload=message.payload.decode()
            )
            print("Message from" + data.get("topic") + ": " + data.get("payload"))

        # @self.mqtt.on_subscribe
        # def on_subscribe(client, userdata, mid, qos):
        #     print("Subscribe mid value: " + str(mid))


