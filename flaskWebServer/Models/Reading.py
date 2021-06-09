import json


class Reading:
    def __init__(self, name, value, icon, unit="", threshold=0, min_threshold=0, max_threshold=100):
        self.name = name
        self.value = value
        self.icon = icon
        self.unit = unit
        self.threshold = threshold
        self.min_threshold = min_threshold
        self.max_threshold = max_threshold
        if max_threshold == 0:
            self.max_threshold = value
        if threshold == 0:
            self.threshold = (abs(max_threshold) - abs(min_threshold)) / 2

    def printStatus(self):
        print("Reading: " + self.name)
        print(str(self.value) + " - " + str(self.threshold))

    def getDict(self):
        reading = {
            'name': self.name,
            'value': self.value,
            'icon': self.icon,
            'unit': self.unit,
            'threshold': self.threshold,
            'min_threshold': self.min_threshold,
            'max_threshold': self.max_threshold
        }
        return reading

    def getJson(self):
        jsonStr = self.getDict()
        return json.dumps(jsonStr)
