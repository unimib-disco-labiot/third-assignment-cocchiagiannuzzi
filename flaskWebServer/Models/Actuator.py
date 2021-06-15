import json


class Actuator:
    def __init__(self, name, state, type):
        self.name = name
        self.state = state
        self.type = type

    def setState(self):
        return self.state

    def printStatus(self):
        print("\nActuator: " + self.name)
        print(" state: " + str(self.state))
        print(" type: " + str(self.type))

    def getDict(self):
        actuator = {
            'name': self.name,
            'state': self.state,
            'type': self.type
        }
        return actuator

    def getJson(self):
        jsonStr = self.getDict()
        return json.dumps(jsonStr)
