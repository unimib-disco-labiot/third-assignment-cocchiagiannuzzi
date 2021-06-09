import json


class Sensor:
    def __init__(self, name, readings):
        self.name = name
        self.readings = readings

    def getReading(self, name):
        for reading in self.readings:
            if reading.name == name:
                return reading

    def printStatus(self):
        print("\nSensor: " + self.name)
        for reading in self.readings:
            print(reading.printStatus())

    def getDict(self):
        sensor = {
            'name': self.name,
            'readings': [
            ]
        }
        for reading in self.readings:
            sensor.get('readings').append(reading.getDict())
        return sensor

    def getJson(self):
        jsonStr = self.getDict()
        return json.dumps(jsonStr)
