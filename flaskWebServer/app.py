import json

from flask import Flask
from flask_mqtt import Mqtt

import topicName
import utility
import views


global devices
devices = []
global influx
influx = True

application = Flask(__name__)
application.register_blueprint(views.home_monitoring)

with open('config.json') as config_file:
    config_data = json.load(config_file)

mqtt_settings = config_data['mqtt_settings']
application.config.update(mqtt_settings)

mqtt = Mqtt(application)



@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected with result code " + str(rc), end="\n\n\n")
        subscribe(topicName.data_update+"/#")
        subscribe(topicName.new_client)
        subscribe(topicName.remove_client)
        subscribe(topicName.actuator_state+"/#")
        if devices == []:
            publish(topicName.all_client_get, "")
    else:
        print("Bad connection. Returned code=", rc)


@mqtt.on_disconnect()
def handle_disconnect(client, userdata, rc):
    print("Disconnecting reason: " + str(rc))


@mqtt.on_message()
def handle_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print("Message from " + data.get("topic") + ":\n " + data.get("payload"))
    utility.handle_topic(data)


@mqtt.on_subscribe()
def handle_subscribe(client, userdata, mid, qos):
    print("Subscribe mid value: " + str(mid))


def subscribe(topic, qos=0):
    mqtt.subscribe(topic, qos)


def publish(topic, message):
    mqtt.publish(topic, message)
    print("\nPublished message: ")
    print(message, end="\n\n")
    print("topic: ")
    print(topic, end="\n\n")


if __name__ == '__main__':
    application.run()
    # socketio.run(application, host='127.0.0.1', port=5000, use_reloader=False, debug=True)
