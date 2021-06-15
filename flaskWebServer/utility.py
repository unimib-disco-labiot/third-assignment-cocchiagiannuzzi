import json

from flask import request, render_template

import app
import topicName
import views

from Models.Actuator import Actuator
from Models.Device import Device
from Models.Reading import Reading
from Models.Sensor import Sensor


def getDevice(devices, MAC_address):
    for device in devices:
        if device.MAC_address == MAC_address:
            return device
    return None


def getReadingFromJson(jsonStr):
    name = jsonStr.get("name")
    if jsonStr.get("value") is None:
        value = 0
    else:
        value = jsonStr.get("value")
    icon = jsonStr.get("icon")
    unit = jsonStr.get("unit")
    if jsonStr.get("threshold"):
        threshold = jsonStr.get("threshold")
        min_threshold = jsonStr.get("min_threshold")
        max_threshold = jsonStr.get("max_threshold")
        return Reading(name, value, icon, unit, threshold, min_threshold, max_threshold)
    return Reading(name, value, icon, unit)


def getSensorsFromJson(sensorsList):
    sensors = []
    for i in range(len(sensorsList)):
        readingsList = sensorsList[i].get("readings")
        readings = []
        if readingsList:
            for j in range(len(readingsList)):
                readings.append(getReadingFromJson(readingsList[j]))
        sensor = Sensor(sensorsList[i].get("name"), readings)
        sensors.append(sensor)
    return sensors


def getActuatorFromJson(jsonStr):
    name = jsonStr.get("name")
    state = jsonStr.get("state")
    type = jsonStr.get("type")
    return Actuator(name, state, type)


def handle_topic(data):
    topic = data.get("topic")
    payload = json.loads(data.get("payload"))
    if topic.startswith(topicName.data_update):
        if app.devices:
            update_sensor(payload)
    elif topic.startswith(topicName.new_client):
        add_device(payload)
    elif topic.startswith(topicName.remove_client):
        remove_device(payload)
    elif topic.startswith(topicName.actuator_state_set):
        setActuatorState(payload)


def setActuatorState(payload):
    device = getDevice(app.devices, payload.get("mac_address"))
    actuator = device.getActuator(payload.get("actuator").get("name"))
    if payload.get("actuator"):
        actuator.state = payload.get("actuator").get("state")
    pass


def remove_device(payload):
    device = getDevice(app.devices, payload.get("mac_address"))
    if device:
        app.devices.remove(device)


def add_device(payload):
    if getDevice(app.devices, payload.get("mac_address")):
        print("Device already registered")
    else:
        sensorsJson = payload.get("sensors")
        sensors = []
        if sensorsJson:
            sensors = getSensorsFromJson(sensorsJson)
        actuatorsJson = payload.get("actuators")
        actuators = []
        if actuatorsJson:
            for i in range(len(actuatorsJson)):
                actuators.append(getActuatorFromJson(actuatorsJson[i]))
        app.devices.append(Device(payload.get("mac_address"), sensors, actuators))
    pass


def update_sensor(payload):
    i = 0
    device = getDevice(app.devices, payload.get("mac_address"))
    if device:
        sensor = device.getSensor(payload.get("name"))
        if sensor:
            for reading in sensor.readings:
                if i < len(payload.get("readings")):
                    reading.value = payload.get("readings")[i].get('value')
                i += 1
    pass
