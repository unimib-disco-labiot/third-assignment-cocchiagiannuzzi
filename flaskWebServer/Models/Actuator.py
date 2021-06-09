import json


class Actuator:
    def __init__(self, name, state):
        self.name = name
        self.state = state

    def setState(self):
        return self.state

    def printStatus(self):
        print("\nActuator: " + self.name)
        print("state: " + str(self.state))

    def getDict(self):
        actuator = {
            'name': self.name,
            'state': self.state
        }
        return actuator

    def getJson(self):
        jsonStr = self.getDict()
        return json.dumps(jsonStr)
