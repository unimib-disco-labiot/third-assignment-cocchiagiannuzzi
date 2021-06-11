import json


class Device:
    def __init__(self, MAC_address, sensors, actuators, switchInflux=True, switchMYSQL=True):
        self.MAC_address = MAC_address
        self.sensors = sensors
        self.actuators = actuators
        self.switchInflux = switchInflux
        self.switchMYSQL = switchMYSQL

    def getSensor(self, name):
        for sensor in self.sensors:
            if sensor.name == name:
                return sensor

    def getActuator(self, name):
        for actuator in self.actuators:
            if actuator.name == name:
                return actuator

    def printStatus(self):
        print("\n\nDevice: " + self.MAC_address)
        print("switchInflux: " + self.switchInflux + " switchMYSQL: " + self.switchMYSQL)
        for sensor in self.sensors:
            print(sensor.printStatus())
        for actuator in self.actuators:
            print(actuator.printStatus())

    def getDict(self):
        device = {
            'mac_address': self.MAC_address,
            'sensors': [
            ],
            'switchInflux': self.switchInflux,
            'switchMYSQL': self.switchMYSQL
        }
        for sensor in self.sensors:
            device.get('sensors').append(sensor.getDict())
        for actuator in self.actuators:
            device.get('actuators').append(actuator.getDict())
        return device

    def getJson(self):
        jsonStr = self.getDict()
        return json.dumps(jsonStr)


